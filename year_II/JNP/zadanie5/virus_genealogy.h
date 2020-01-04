#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include <exception>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <iostream>

class VirusNotFound : public std::exception {
public:
  const char *what() const noexcept { return "VirusNotfound"; }
};

class VirusAlreadyCreated : public std::exception {
public:
  const char *what() const noexcept { return "VirusAlreadyCreated"; }
};

class TriedToRemoveStemVirus : public std::exception {
public:
  const char *what() const noexcept { return "TriedToRemoveStemVirus"; }
};

template <class Virus> class VirusGenealogy {
private:
    typedef  typename Virus::id_type Vir;
    struct Node {
        Virus virus;
        std::set<std::shared_ptr<Node>, std::owner_less<std::shared_ptr<Node>>> parents_;
        std::set<std::shared_ptr<Node>, std::owner_less<std::shared_ptr<Node>>> children_;

        Node(typename Virus::id_type id)
            : virus(id) {}
    };
    
    typename Virus::id_type const stem_id;
    std::map<typename Virus::id_type, std::shared_ptr<Node>> viruses_;

public:
    VirusGenealogy(typename Virus::id_type const &stem_id)
        : stem_id(stem_id) {
        std::shared_ptr<Node> ptr = std::make_shared<Node>(stem_id);
        viruses_.insert(std::make_pair(stem_id, ptr));
    };
  
    typename Virus::id_type get_stem_id() const noexcept { return stem_id; }
    
    VirusGenealogy(const VirusGenealogy &that) = delete;
    
    VirusGenealogy &operator=(const VirusGenealogy &) = delete;
    
    std::vector<typename Virus::id_type>
    get_children(typename Virus::id_type const &id) const {
        /* Potentially constructor may throw an exception, 
           but it is no problem. If comparison throws exception:
           it will just go further, provided it is not a subclass
           of out_of_range. */
        try {
            auto children__ = viruses_.at(id)->children_;
            std::vector<typename Virus::id_type> res;
            for (auto child__ : children__) {
                res.push_back(child__->virus.get_id());
            }
            return res;
        } catch (std::out_of_range &oor) {
            throw VirusNotFound();
        }
    }
    
    std::vector<typename Virus::id_type>
    get_parents(typename Virus::id_type const &id) const {
        /* Potentially constructor may throw an exception, 
           but it is no problem  */
        try {
            auto parents__ =  viruses_.at(id)->parents_;
            std::vector<typename Virus::id_type> res;
            for (auto parent__ : parents__) {
                res.push_back(parent__->virus.get_id());
            }
            return res;
        } catch (std::out_of_range &oor) {
            throw VirusNotFound();
        }
    }

    bool exists(typename Virus::id_type const &id) const  {
        return viruses_.find(id) != viruses_.end();
    }
    
    Virus &operator[](typename Virus::id_type const &id) const {
        if (exists(id)) {
            return viruses_.at(id)->virus;
        } else {
            throw VirusNotFound();
        }
    }

    
    void create(typename Virus::id_type const &id,
                typename Virus::id_type const &parent_id) {
        create(id, std::vector<typename Virus::id_type>({parent_id}));
    }
    
    void create(typename Virus::id_type const &id,
                std::vector<typename Virus::id_type> const &parent_ids) {
        if (exists(id)) {
            throw VirusAlreadyCreated();
        }

        if (parent_ids.empty()) {
            throw VirusNotFound();
        }

        std::shared_ptr<Node> ptr = std::make_shared<Node>(id);
        
        for (auto parent_id : parent_ids) {
            if (!exists(parent_id)) {
                throw VirusNotFound();
            }
        }

        viruses_.insert(std::make_pair(id, ptr));

        for (auto it = parent_ids.begin(); it != parent_ids.end(); it++) {
            try {
                connect(id, *it);
            } catch (...) {
                remove(id);
                throw;
            }
            
        }
    }

        
    void connect(typename Virus::id_type const &child_id,
                 typename Virus::id_type const &parent_id) {
        if (!exists(parent_id) || !exists(child_id)) {
            throw VirusNotFound();
        }

        auto parent_children = viruses_.at(parent_id)->children_;
        auto child_parents = viruses_.at(child_id)->parents_;
        auto parent_ptr = viruses_.at(parent_id);
        auto child_ptr = viruses_.at(child_id);

        
        parent_children.insert(viruses_.at(child_id));
        child_parents.insert(viruses_.at(parent_id));
        parent_ptr->children_ = parent_children;
        child_ptr->parents_ = child_parents;
    }
    
    void remove(typename Virus::id_type const &id) {
        if (!exists(id)) {
            throw VirusNotFound();
        }
        
        if (id == stem_id) {
            throw TriedToRemoveStemVirus();
        }

        auto node_ptr = viruses_.at(id);
        
        for (auto parent_node : node_ptr->parents_) {
            auto &parent_children = parent_node->children_;
            parent_children.erase(node_ptr);
        }
        
        for (auto child_node : node_ptr->children_) {
            auto &child_parents = child_node->parents_;
            
            child_parents.erase(node_ptr);
            if (child_parents.size() == 0) {
                remove(child_node->virus.get_id());
            }
        }
        
        viruses_.erase(id);
    }
    
};

#endif /* VIRUS_GENEALOGY_H */
