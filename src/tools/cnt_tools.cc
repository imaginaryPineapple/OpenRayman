#include <tools/cnt_tools.h>
#include <tools/common_tools.h>
#include <openrayman/cnt_archive.h>
#include <cstdlib>

namespace openrayman
{
    namespace cnt_tools
    {
        int extract(const std::string& path, const std::string& name_in, const std::string& target)
        {
            std::ifstream stream(path, std::ifstream::binary);
            cnt_archive archive(stream);
            if(!archive.valid())
                return fail_and_print("Operation failed");
            cnt_file* file;
            if((file = archive.archive_node().find_file(name_in)) == nullptr)
                return fail_and_print("Operation failed");
            std::ofstream output(target, std::ofstream::binary);
            if(!output.is_open())
                return fail_and_print("Operation failed");
            std::size_t at = 0;
            while(at < file->size)
            {
                std::vector<char> read = file->read(at, 1024 * 4096);
                output.write((char*)read.data(), read.size());
                at += read.size();
                if(read.size() == 0)
                    break;
            }
            return EXIT_SUCCESS;
        }

        void print_recursive(cnt_directory_node& node, int indent)
        {
            std::string indent_str(indent, ' ');
            std::cout << indent_str << node.name << " -> " << std::endl;
            for(cnt_file& file : node.local_files())
            {
                std::cout << indent_str << "    " << file.name << " -> " << std::endl;
                std::cout << indent_str << "    " << "    " << (file.size / 1024.0) << " KB" << std::endl;
            }
            for(std::unique_ptr<cnt_directory_node>& child : node.local_children())
                print_recursive(*child, indent + 4);
        }

        int print_hierarchy(const std::string& path)
        {
            std::ifstream stream(path, std::ifstream::binary);
            cnt_archive archive(stream);
            if(!archive.valid())
                return fail_and_print("Operation failed");
            print_recursive(archive.archive_node(), 0);
            return EXIT_SUCCESS;
        }
    }
}
