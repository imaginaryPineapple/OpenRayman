#include <tools/cnt_tools.h>
#include <data_extractor/cnt/cnt_archive.h>
#include <tools/common_tools.h>
#include <cstdlib>

namespace openrayman
{
    namespace cnt_tools
    {
        int extract(const std::string& path, const std::string& name_in, const std::string& target)
        {
            cnt_archive archive(path);
            if(!archive.valid())
                return fail_and_print("Operation failed");
            cnt_file* file;
            if((file = archive.get_archive_node().find_file(name_in)) == nullptr)
                return fail_and_print("Operation failed");
            std::ofstream output(target, std::ofstream::out | std::ofstream::binary);
            if(!output.is_open())
                return fail_and_print("Operation failed");
            std::size_t at = 0;
            while(at < file->size)
            {
                std::vector<std::uint8_t> read = file->read(at, 1024 * 4096);
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
            for(cnt_file& file : node.get_local_files())
            {
                std::cout << indent_str << "    " << file.name << " -> " << std::endl;
                std::cout << indent_str << "    " << "    " << (file.size / 1024.0) << " KB" << std::endl;
            }
            for(cnt_directory_node& child : node.get_local_children())
                print_recursive(child, indent + 4);
        }

        int print_hierarchy(const std::string& path)
        {
            cnt_archive archive(path);
            if(!archive.valid())
                return fail_and_print("Operation failed");
            print_recursive(archive.get_archive_node(), 0);
            return EXIT_SUCCESS;
        }
    }
}
