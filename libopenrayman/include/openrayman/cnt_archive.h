#ifndef OPENRAYMAN_CNT_ARCHIVE_H
#define OPENRAYMAN_CNT_ARCHIVE_H

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

    class cnt_directory_node
    {
public:
        cnt_directory_node(cnt_directory_node* parent, cnt_archive& owner, const std::string& name, std::int32_t cnt_index = -1) :
            owner(owner), parent(parent), name(name), cnt_index(cnt_index)
        {
        }

        // Pushes a new directory as a child of this node.
        void push_child(const std::string& child);
        void push_child(const std::string& child, std::int32_t cnt_index);

        // Pushes a new file as a child of this node.
        void push_file(cnt_directory_node& parent, const std::string& name, std::int32_t ptr_in_archive, std::int32_t size, std::vector<char>& xor_key);

        // Returns all children of this directory node, recursively.
        std::vector<cnt_directory_node*> children();

        // Returns a reference to all files that are children of this node.
        std::vector<cnt_file*> files();

        // Returns a reference to all children that are explicit children of this node.
        inline std::vector<cnt_directory_node>& local_children()
        {
            return m_children;
        }

        // Returns a reference to all files that are explicit children of this node.
        inline std::vector<cnt_file>& local_files()
        {
            return m_files;
        }

        // Returns the full path specifier of this directory.
        std::string full_path() const;

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

        // The archive that this directory resides in.
        cnt_archive& owner;

        // The parent directory of this directory.
        // No (nullptr) parent indicates that this is a top-level node.
        cnt_directory_node* parent;

        // The local name of this directory, e.g. "/" or "textures_objet".
        std::string name;

        // The name that was extracted from the archive before deserialization.
        std::string cnt_raw_name;

        // The index that was extracted from the archive before deserialization.
        std::int32_t cnt_index;
private:
        std::vector<cnt_directory_node> m_children;
        std::vector<cnt_file> m_files;
    };

    class cnt_file
    {
public:
        cnt_file(cnt_directory_node& parent, const std::string& name, std::int32_t ptr_in_archive, std::size_t size, std::vector<char>& xor_key) :
            parent(parent), name(name), size(size), m_ptr_in_archive(ptr_in_archive), m_xor_key(xor_key)
        {
        }

        // Returns the full path specifier of this file.
        std::string full_path() const;

        // Reads a max of "count" bytes from this file, starting at "start".
        std::vector<char> read(std::int32_t start, std::int32_t count) const;

        // The directory that this file resides in.
        cnt_directory_node& parent;

        // The local name of this file, e.g. "VigLoadAstro01.gf".
        std::string name;

        // The name that was extracted from the archive before deserialization.
        std::string cnt_raw_name;

        // The size, in bytes, that this file occupies in the archive.
        std::size_t size;
private:
        std::int32_t m_ptr_in_archive;
        std::vector<char> m_xor_key;
    };

    // CNT archives are used for storing textures.
    class cnt_archive
    {
        friend cnt_file;
public:
        cnt_archive(std::istream& stream);

        // Returns true if the archive was loaded successfully and was valid.
        bool valid() const
        {
            return m_valid;
        }

        // Returns a reference to the top most directory node, known as "/".
        inline cnt_directory_node& archive_node()
        {
            return m_archive_node;
        }
private:
        cnt_directory_node m_archive_node;
        std::istream& m_stream;
        bool m_valid;
    };
}

#endif
