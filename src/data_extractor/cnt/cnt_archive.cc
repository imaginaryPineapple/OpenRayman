#include <data_extractor/cnt/cnt_archive.h>

namespace openrayman
{
    std::string cnt_file::get_full_path() const
    {
        return parent.get_full_path() + "/" + name;
    }

    cnt_file* cnt_directory_node::find_file(const std::string& name)
    {
        cnt_directory_node& archive_node = owner.get_archive_node();
        if(name.find(archive_node.name) == 0)
            return archive_node.find_file(std::string(name).erase(0, archive_node.name.size()));

        std::string in_this = std::string(name).substr(0, name.find("/"));
        for(cnt_file& file : m_files)
        {
            if(file.name == in_this)
                return &file;
        }
        for(cnt_directory_node& child : m_children)
        {
            if(child.name == in_this)
                return child.find_file(std::string(name).erase(0, std::min(name.size(), in_this.size() + 1)));
        }
        return nullptr;
    }

    cnt_directory_node* cnt_directory_node::find_child(const std::string& name)
    {
        if(name == "")
            return this;

        cnt_directory_node& archive_node = owner.get_archive_node();
        if(name.find(archive_node.name) == 0)
            return archive_node.find_child(std::string(name).erase(0, archive_node.name.size()));

        std::string in_this = std::string(name).substr(0, name.find("/"));
        for(cnt_directory_node& child : m_children)
        {
            if(child.name == in_this)
                return child.find_child(std::string(name).erase(0, std::min(name.size(), in_this.size() + 1)));
        }
        return nullptr;
    }

    std::vector<std::uint8_t> cnt_file::read(std::int32_t start, std::int32_t count) const
    {
        std::vector<std::uint8_t> read(0, 0);
        read.reserve(count);
        std::size_t start_in_archive = ptr_in_archive + start;
        std::size_t end = ptr_in_archive + start + size;
        std::size_t pos = start_in_archive;

        parent.owner.get_raw_stream().seekg(pos);
        std::uint8_t c;
        while(pos < end && pos < ptr_in_archive + start + count)
        {
            if(!parent.owner.get_raw_stream().read((char*)&c, 1))
                break;
            c ^= xor_key[(pos - start_in_archive) % 4];
            read.push_back(c);
            pos++;
        }

        return read;
    }

    cnt_archive::cnt_archive(const std::string& path) :
        cnt_archive(*(new std::ifstream(path, std::ifstream::in | std::ifstream::binary)))
    {

    }

    cnt_archive::cnt_archive(std::ifstream& stream) :
        m_archive_node(*(new cnt_directory_node(*this, nullptr, "/"))), m_stream(stream), m_valid(false)
    {
        if(!stream.is_open())
            return;

        std::int32_t num_directories, num_files;
        m_stream.read((char*)&num_directories, sizeof(std::int32_t));
        m_stream.read((char*)&num_files, sizeof(std::int32_t));
        if(num_directories <= 0 && num_files <= 0)
            return;

        char signature[2];
        m_stream.read(signature, 2);
        if(signature[0] != 0x1 || signature[1] != 0x1)
            return;

        char xor_key;
        m_stream.read(&xor_key, 1);

        for(std::int32_t dir = 0; dir < num_directories; dir++)
        {
            std::int32_t dir_name_len;
            m_stream.read((char*)&dir_name_len, sizeof(std::int32_t));
            std::string dir_name = "";
            for(std::int32_t n = 0; n < dir_name_len; n++)
            {
                char c;
                m_stream.read(&c, 1);
                dir_name += c ^ xor_key;
            }
            std::vector<std::string> components;
            std::size_t last_begun = 0;
            for(std::size_t n = 0; n < dir_name.size(); n++)
            {
                if(dir_name[n] == '\\' || dir_name[n] == '/')
                {
                    if(n - last_begun > 0)
                        components.push_back(dir_name.substr(last_begun, n - last_begun));
                    n++;
                    last_begun = n;
                }
            }
            if((dir_name.size() - 1 - last_begun) > 0)
                components.push_back(dir_name.substr(last_begun, dir_name.size() - 1 - last_begun));
            cnt_directory_node* parent = &m_archive_node;
            for(std::size_t n = 0; n < components.size() - 1; n++)
            {
                if(!parent->has_child(components[n]))
                    parent->push_child(components[n]);
                parent = parent->find_child(components[n]);
            }
            parent->push_child(components[components.size() - 1], dir);
        }

        std::vector<cnt_directory_node*> all_directories = m_archive_node.get_children();
        char tmp;
        m_stream.read(&tmp, 1);

        for(std::int32_t file = 0; file < num_files; file++)
        {
            std::int32_t dir_index, file_name_len;
            m_stream.read((char*)&dir_index, sizeof(std::int32_t));
            m_stream.read((char*)&file_name_len, sizeof(std::int32_t));
            cnt_directory_node* parent = &m_archive_node;
            for(cnt_directory_node* node : all_directories)
            {
                if(node->cnt_index == dir_index)
                    parent = node;
            }
            std::string file_name = "";
            for(std::int32_t n = 0; n < file_name_len; n++)
            {
                char c;
                m_stream.read(&c, 1);
                file_name += c ^ xor_key;
            }
            char f_xor_key[4];
            m_stream.read(f_xor_key, 4);
            std::int32_t tmp, ptr_in_archive, file_size;
            m_stream.read((char*)&tmp, sizeof(std::int32_t));
            m_stream.read((char*)&ptr_in_archive, sizeof(std::int32_t));
            m_stream.read((char*)&file_size, sizeof(std::int32_t));
            parent->push_file(*parent, file_name, ptr_in_archive, file_size, f_xor_key);
        }

        m_valid = true;
    }
}
