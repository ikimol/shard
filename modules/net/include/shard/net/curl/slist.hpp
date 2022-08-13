// Copyright (c) 2022 Miklos Molnar. All rights reserved.

#ifndef SHARD_NET_CURL_SLIST_HPP
#define SHARD_NET_CURL_SLIST_HPP

#include <curl/curl.h>

#include <string>

namespace shard::net::curl {

class slist {
public:
    /// Default constructor
    slist() = default;

    /// Construct using a pointer to a curl string list
    ///
    /// \note This will deep copy the string list. The argument's memory will
    /// *NOT* be managed.
    ///
    /// \param slist The curl string list struct
    explicit slist(curl_slist* slist) { copy(slist); }

    /// Assign from a pointer to a curl string list
    ///
    /// \note This will free the currently managed list and deep copy the
    /// argument. The argument's memory will *NOT* be managed.
    ///
    /// \param slist The curl string list struct
    slist& operator=(curl_slist* slist) noexcept {
        clear();
        copy(slist);
        return *this;
    }

    /// Destroy the curl string list freeing up memory
    ~slist() { clear(); }

    /// Append a string to the list
    void append(const std::string& string) { m_list = curl_slist_append(m_list, string.c_str()); }

    /// Remove every item and free the memory used by the curl string list
    void clear() {
        if (m_list) {
            curl_slist_free_all(m_list);
            m_list = nullptr;
        }
    }

    /// Get the underlying curl string list
    curl_slist* get() const { return m_list; }

private:
    void copy(curl_slist* slist) {
        while (slist) {
            m_list = curl_slist_append(m_list, slist->data);
            slist = slist->next;
        }
    }

private:
    curl_slist* m_list = nullptr;
};

} // namespace shard::net::curl

#endif // SHARD_NET_CURL_SLIST_HPP
