
/*
    SiVector.h
*/

#pragma once

#include "Logging.h"

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

#define Vi Vi
namespace Vi {

    using ID = size_t;
    constexpr ID InvalidID = (size_t)std::numeric_limits<size_t>::max();

    template <typename type>
    class SiVector {
    public:
        ID validate_id(const ID element_id) const {
            if (is_valid_id(element_id))
                return element_id;
            return InvalidID;
        }

        size_t size() const {
            return data.size();
        }
        
        ID add(const type& element) {
            if (id_from_index.size() > data.size()) {
                size_t element_index = data.size();
                data.push_back(element);
                ID element_id = id_from_index[element_index];
                return element_id;
            }
            size_t element_id = data.size();
            data.push_back(element);
            index_from_id.push_back(element_id);
            id_from_index.push_back(element_id);
            return element_id;
        }

        ID add(type&& element) {
            if (id_from_index.size() > data.size()) {
                size_t element_index = data.size();
                data.push_back(std::move(element));
                ID element_id = id_from_index[element_index];
                return element_id;
            }
            size_t element_id = data.size();
            data.push_back(std::move(element));
            index_from_id.push_back(element_id);
            id_from_index.push_back(element_id);
            return element_id;
        }

        ID remove(const ID element_id) {
            if (!is_valid_id(element_id))
                return InvalidID;

            size_t last_index = data.size() - 1;
            size_t element_index = index_from_id[element_id];

            if (element_index == last_index) {
                data.pop_back();
                return element_id;
            }

            std::swap(data[element_index], data[last_index]);
            std::swap(id_from_index[element_index], id_from_index[last_index]);
            std::swap(index_from_id[id_from_index[element_index]], index_from_id[id_from_index[last_index]]);

            data.pop_back();
            return element_id;
        }
        
        type& get(const ID element_id) {
            if (!is_valid_id(element_id)) {
                Log::error("get(): Invalid ID: " + std::to_string(element_id));
                std::terminate();
            }
            size_t element_index = index_from_id[element_id];
            return data[element_index];
        }

        const type& get(const ID element_id) const {
            if (!is_valid_id(element_id)) {
                Log::error("get(): Invalid ID: " + std::to_string(element_id));
                std::terminate();
            }
            size_t element_index = index_from_id[element_id];
            return data[element_index];
        }
        
        type& operator [] (const size_t element_index) {
            return data[element_index];
        }

        const type& operator [] (const size_t element_index) const {
            return data[element_index];
        }
        
        auto begin() { return data.begin(); }
        auto end()   { return data.end(); }
        auto begin() const { return data.begin(); }
        auto end() const   { return data.end(); }
    
    private:
        std::vector<size_t> index_from_id{};
        std::vector<size_t> id_from_index{};
        std::vector<type> data{};
        
        bool is_valid_id(const ID element_id) const {
            if (element_id >= index_from_id.size())
                return false;
            size_t element_index = index_from_id[element_id];
            if (element_index >= data.size())
                return false;
            return true;
        }
    };
}

