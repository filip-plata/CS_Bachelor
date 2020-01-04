#include <iostream>

#include <algorithm>
#include <deque>
#include <unordered_map>
#include <climits>
#include <cassert>

#include "strdeque.h"

using namespace std;

namespace jnp1 {

    /* Since its constexpr, it will be intialized on compile time,
     * no fiasco danger.
     *
     * we don't want to force link with emptystrdeque.o, so we have to hardcode it
     * ideally, we'd use a shared, private header but the problem specification
     * forbids it ;(
     */
    static constexpr unsigned long CONST_QUEUE_ID = ULONG_MAX;

    #ifdef NDEBUG
    static constexpr bool debug = true;
    #else
    static constexpr bool debug = false;
    #endif

    /* Returns cerr if we're outputting debug information
     * or a no-op stream if we want no debug */
    static ostream& cerr_wrapper() {
        // cppcheck-suppress unreadVariable
        static ios_base::Init __init;
        if (debug) {
            // no op stream
            // cf. https://stackoverflow.com/questions/25690636/is-it-valid-to-construct-an-stdostream-from-a-null-buffer
            static ostream os(nullptr);
            return os;
        }
        else {
            return cerr;
        }
    }

    // ID numbers are assigned one by one, starting from 0
    static unsigned long& next_deque_id() {
        static unsigned long next_deque_id = 0;
        return next_deque_id;
    }

    // Deque store.
    static unordered_map<unsigned long, deque<string>>& deques() {
        static unordered_map<unsigned long, deque<string>> deques = unordered_map<unsigned long, deque<string>>();
        return deques;
    }

    // ######### UTILS

    static deque<string> empty_deque_if_not_present(unsigned long id) {
        auto deque_it = deques().find(id);

        if (deque_it != deques().end()) {
            return deque_it->second;
        }
        else {
            deque<string> deq;
            return deq;
        }
    }

    static void assert_wrapper(bool expr) {
        if (debug) {
            assert(expr);
        }
    }

    // ######## END UTILS

    unsigned long strdeque_new() {
        unsigned long id = next_deque_id(); //changed int to unsigned long
        assert_wrapper(id != CONST_QUEUE_ID);

        cerr_wrapper() << "strdeque_new: Creating new queue, with id=" << id << "\n";

        ++next_deque_id();
        deques()[id] = deque<string>();
        return id;
    }

    void strdeque_delete(unsigned long id) {

        if (id == CONST_QUEUE_ID) {
            cerr_wrapper() << "Trying to delete a constant queue... No-op\n";
            return;
        }

        auto deque_it = deques().find(id);

        if (deque_it != deques().end()) {
            cerr_wrapper() << "strdeque_delete: Deleting the queue with id=" << id
                           << "\n";
            deques().erase(deque_it);
        } else {
            cerr_wrapper() << "strdeque_delete: The the queue with id=" << id <<
                    " does not exist, no op\n";
        }
    }

    size_t strdeque_size(unsigned long id) {
        auto deque_it = deques().find(id);

        if (deque_it != deques().end()) {
            cerr_wrapper() << "strdeque_size: Size of the queue with id=" << id <<
                    " is " << deque_it->second.size() << "\n";
            return deque_it->second.size();
        }
        else {
            cerr_wrapper() << "strdeque_size: The the queue with id=" << id <<
                    " does not exist, assuming the size equals 0\n";
            return 0;
        }


    }

    void strdeque_insert_at(unsigned long id, size_t pos, const char *value) {
        if (value == nullptr) {
            cerr_wrapper() << "strdeque_insert_at: ignoring an attempt to insert a "
                    "nullptr into a deque\n";
            return;
        } else if (id == CONST_QUEUE_ID) {
            cerr_wrapper() << "strdeque_insert_at: ignoring an attempt to insert "
                    "into a const deque\n";
            return;
        }

        auto deque_it = deques().find(id);

        if (deque_it == deques().end()) {

            cerr_wrapper() << "strdeque_insert_at: The the queue with id=" << id <<
                    " does not exist, no op\n";

        } else {
            auto position = deque_it->second.begin() + pos;
            string s(value);

            if (position > deque_it->second.end()) {
                position = deque_it->second.end();
            }

            cerr_wrapper() << "strdeque_insert_at: inserting the string `"
                 << value
                 << "\' at position "
                 << distance(deque_it->second.begin(), position)
                 << " to the queue with id=" << id
                 << "\n";

            deque_it->second.insert(position, s);
        }
    }

    void strdeque_remove_at(unsigned long id, size_t pos) {
        auto deque_it = deques().find(id);

        if (deque_it == deques().end()) {
            cerr_wrapper() << "strdeque_remove_at: The the queue with id=" << id <<
                    " does not exist, no op\n";
        } else if (id == CONST_QUEUE_ID) {
            cerr_wrapper() << "strdeque_remove_at: ignoring an attempt to remove "
                    "from a const deque\n";
            return;
        }

        if (deque_it != deques().end()) {
            auto position = deque_it->second.begin() + pos;


            if (position < deque_it->second.end()) {
                cerr_wrapper() << "strdeque_remove_at: removing the element "
                     << "at position "
                     << distance(deque_it->second.begin(), position)
                     << " from the queue with id=" << id
                     << "\n";
            } else {
                cerr_wrapper() << "strdeque_remove_at: trying to remove the element"
                               << " at position "
                               << distance(deque_it->second.begin(), position)
                               << " which is invalid. No op\n";
            }

            if (position < deque_it->second.end()) {
                deque_it->second.erase(position);
            }
        }
    }

    /* The const char* pointer points to the string kept within a structure.
     * It's the user's concern and obligation to copy it themselves if they
     * need it later. We're not copying it ourselves since it would be useless
     * memory management and performance burden.
     *
     * If this were a real library, this would be written in the documentation
     */
    const char *strdeque_get_at(unsigned long id, size_t pos) {

        if (id == CONST_QUEUE_ID) {
            cerr_wrapper() << "strdeque_get_at: ignoring an attempt to get "
                    "from a const deque\n";
            return nullptr;
        }

        auto deque_it = deques().find(id);

        if (deque_it != deques().end()) {
            auto position = deque_it->second.begin() + pos;

            if (position >= deque_it->second.end()) {
                cerr_wrapper() << "strdeque_get_at: trying to get the element"
                               << " at position "
                               << distance(deque_it->second.begin(), position)
                               << " which is invalid. NULL returned\n";
                return nullptr;
            }
            else {
                cerr_wrapper() << "strdeque_get_at: returning the element "
                     << "at position "
                     << distance(deque_it->second.begin(), position)
                     << " from the queue with id=" << id
                     << "\n";
                return deque_it->second.at(pos).c_str();
            }
        } else {
            cerr_wrapper() << "strdeque_get_at: The the queue with id=" << id <<
                " does not exist, no op\n";
        }

        return nullptr;
    }

    void strdeque_clear(unsigned long id) {

        if (id == CONST_QUEUE_ID) {
            cerr_wrapper() << "strdeque_remove_at: ignoring an attempt to remove "
                    "from a const deque\n";
            return;
        }

        auto deque_it = deques().find(id);

        if (deque_it != deques().end()) {
            deque_it->second.clear();

            cerr_wrapper() << "strdeque_clear: The the queue with id=" << id <<
                " is cleared\n";
        } else {
            cerr_wrapper() << "strdeque_clear: The the queue with id=" << id <<
                " does not exist, no op\n";
        }
    }

    int strdeque_comp(unsigned long id1, unsigned long id2) {

        cerr_wrapper() << "strdeque_comp: trying to obtain queues."
                       << "Empty deque is used if id is not present\n";

        auto deque1 = empty_deque_if_not_present(id1);
        auto deque2 = empty_deque_if_not_present(id2);

        if (deque1 < deque2) {
            cerr_wrapper() << "strdeque_comp: Second deque is greater\n";
            return -1;
        }
        if (deque1 > deque2) {
            cerr_wrapper() << "strdeque_comp: first deque is greater\n";
            return 1;
        }

        cerr_wrapper() << "strdeque_comp: deques all equal\n";

        return 0;
    }
}
