#ifndef CNT_ARCHIVE_H
#define CNT_ARCHIVE_H

#include <data_extractor/data_decoder.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace openrayman
{
    // Forward declare
    class cnt_archive;
    class cnt_file;
    class cnt_directory_node;

    // CNT archives are used for storing textures.
    // This is all heavily adapted from Rayman2Lib.
    class cnt_archive
    {
public:
        cnt_archive(const std::string& path);
        cnt_archive(std::ifstream& stream);

        ~cnt_archive()
        {
            if(m_stream.is_open())
                m_stream.close();
        }

        // Returns true if the archive was loaded successfully and was valid.
        bool valid() const
        {
            return m_valid;
        }

        // Returns a reference to the top most directory node, known as "/".
        inline cnt_directory_node& get_archive_node()
        {
            return m_archive_node;
        }

        // Returns a reference to underlying stream used by the CNT archive.
        inline std::ifstream& get_raw_stream()
        {
            return m_stream;
        }
private:
        cnt_directory_node& m_archive_node;
        std::ifstream& m_stream;
        bool m_valid;
    };

    class cnt_file
    {
public:
        cnt_file(cnt_directory_node& parent, const std::string& name, std::int32_t ptr_in_archive, std::int32_t size, char* xor_key) :
            parent(parent), name(name), ptr_in_archive(ptr_in_archive), size(size)
        {
            for(std::size_t n = 0; n < 4; n++)
                this->xor_key[n] = xor_key[n];
        }

        // Returns the full path specifier of this file.
        // Cant inline because incomplete.
        std::string get_full_path() const;

        // Reads a max of "count" bytes from this file, starting at "start".
        std::vector<std::uint8_t> read(std::int32_t start, std::int32_t count) const;

        cnt_directory_node& parent;
        std::string name;
        std::int32_t size;
private:
        std::int32_t ptr_in_archive;
        char xor_key[4];
    };


    class cnt_directory_node
    {
        friend class cnt_archive;
public:
        cnt_directory_node(cnt_archive& owner, cnt_directory_node* parent, const std::string& name, std::int32_t cnt_index = -1) :
            owner(owner), parent(parent), name(name), cnt_index(cnt_index)
        {

        }

        // Pushes a new directory as a child of this node.
        inline void push_child(const std::string& child)
        {
            m_children.push_back(cnt_directory_node(owner, this, child));
        }

        // Pushes a new directory as a child of this node.
        inline void push_child(const std::string& child, std::int32_t cnt_index)
        {
            m_children.push_back(cnt_directory_node(owner, this, child, cnt_index));
        }

        // Pushes a new file as a child of this node.
        inline void push_file(cnt_directory_node& parent, const std::string& name, std::int32_t ptr_in_archive, std::int32_t size, char* xor_key)
        {
            m_files.push_back(cnt_file(parent, name, ptr_in_archive, size, xor_key));
        }

        // Returns all children of this directory node, recursively.
        inline std::vector<cnt_directory_node*> get_children()
        {
            std::vector<cnt_directory_node*> values;
            for(cnt_directory_node& node : m_children)
            {
                values.push_back(&node);
                std::vector<cnt_directory_node*> node_children = node.get_children();
                for(std::size_t n = 0; n < node_children.size(); n++)
                    values.push_back(node_children[n]);
            }
            return values;
        }

        // Returns a reference to all files that are children of this node.
        inline std::vector<cnt_file*> get_files()
        {
            std::vector<cnt_file*> values;
            for(cnt_file& file : m_files)
                values.push_back(&file);
            for(cnt_directory_node* child : get_children())
            {
                for(cnt_file& file : child->m_files)
                    values.push_back(&file);
            }
            return values;
        }

        // Returns a reference to all children that are explicit children of this node.
        inline std::vector<cnt_directory_node>& get_local_children()
        {
            return m_children;
        }

        // Returns a reference to all files that are explicit children of this node.
        inline std::vector<cnt_file>& get_local_files()
        {
            return m_files;
        }

        // Returns the full path specifier of this directory.
        inline std::string get_full_path() const
        {
            if(parent == nullptr)
                return name;
            if(parent == &owner.get_archive_node())
                return parent->get_full_path() + name;
            return parent->get_full_path() + "/" + name;
        }

        // Returns true if the file exists in this directory.
        inline bool has_file(const std::string& name)
        {
            return find_file(name) != nullptr;
        }

        // Returns true if the subdirectory exists in this directory.
        inline bool has_child(const std::string& name)
        {
            return find_child(name) != nullptr;
        }

        // Returns a reference to the file if it was found, or nullptr if it was not found.
        cnt_file* find_file(const std::string& name);

        // Returns a reference to the child directory if it was found, or nullptr if it was not found.
        cnt_directory_node* find_child(const std::string& name);

        cnt_archive& owner;
        cnt_directory_node* parent;

        std::string name;
        std::int32_t cnt_index;
private:
        std::vector<cnt_directory_node> m_children;
        std::vector<cnt_file> m_files;
    };
}

#endif
