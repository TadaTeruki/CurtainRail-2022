
#include <initializer_list>
#include <iostream>
#include <list>
#include <unordered_set>

namespace curtain_rail {

template <typename LIT> using list_iterator = typename std::list<LIT>::iterator;

template <typename T, unsigned int DIM> class container {

    template <typename UNIT> struct value_cont {
        T value;
        list_iterator<UNIT> table_itr[DIM];
        unsigned int id;

        value_cont(const T val, const unsigned int _id) {
            value = val;
            id = _id;
        }
    };

    struct cr_unit {
        list_iterator<value_cont<cr_unit>> value_src_itr;
        double point;
    };

    class level_map_hash {
      public:
        size_t operator()(const list_iterator<value_cont<cr_unit>> &itr) const { return itr->id; }
    };


    struct range_pointer {
        list_iterator<cr_unit> s_edge[DIM], e_edge[DIM];
        float s_point[DIM], e_point[DIM];
        using level_map_container = std::unordered_set<list_iterator<value_cont<cr_unit>>, level_map_hash>;
        level_map_container level_map;

      public:
        typename level_map_container::iterator begin() { return level_map.begin(); }

        typename level_map_container::iterator end() { return level_map.end(); }

        void recount(list_iterator<value_cont<cr_unit>> value_src_itr, const unsigned int id) {

            for (unsigned int dim = 0; dim < DIM; dim++) {
                const float pt = value_src_itr->table_itr[dim]->point;
                if (s_point[dim] >= pt || pt >= e_point[dim]) {
                    level_map.erase(value_src_itr);
                    return;
                }
            }

            level_map.insert(value_src_itr);
        }
    };

  public:
    using value_src_iterator = list_iterator<value_cont<cr_unit>>;
    using table_iterator = list_iterator<cr_unit>;
    using range_ptr_iterator = list_iterator<range_pointer>;

  private:

    std::list<value_cont<cr_unit>> value_src;
    std::list<cr_unit> table[DIM];
    std::list<range_pointer> range_ptr;
    unsigned int next_id;

    void sequential_search(const unsigned int dim, const double point, table_iterator *r_itr, range_ptr_iterator *rpt) {

        while ((*r_itr) != table[dim].begin()) {
            (*r_itr)--;
            if (rpt != nullptr)
                (*rpt)->recount((*r_itr)->value_src_itr, (*r_itr)->value_src_itr->id);
            if ((*r_itr)->point < point) {
                (*r_itr)++;
                break;
            }
        }

        while ((*r_itr) != table[dim].end()) {
            if ((*r_itr)->point > point) {
                break;
            }
            if (rpt != nullptr)
                (*rpt)->recount((*r_itr)->value_src_itr, (*r_itr)->value_src_itr->id);
            (*r_itr)++;
        }
    }

    void arrange(const unsigned int dim, const float point, table_iterator *next_itr) {

        const table_iterator prev_itr = *next_itr;
        sequential_search(dim, prev_itr->point, next_itr, nullptr);

        if (*next_itr != prev_itr) {
            const cr_unit targ = *prev_itr;
            table[dim].erase(prev_itr);
            (*next_itr) = table[dim].emplace(*next_itr, targ);
        }
    }

    void arrange_for_move(const unsigned int dim, const float old_point, const float new_point, table_iterator *next_itr) {
        arrange(dim, new_point, next_itr);
        value_src_iterator v_itr = (*next_itr)->value_src_itr;

        for (range_ptr_iterator rpt = range_ptr.begin(); rpt != range_ptr.end(); rpt++) {

            const float s_pt = rpt->s_point[dim];
            const float e_pt = rpt->e_point[dim];

            const bool is_inside_before = s_pt <= old_point && old_point <= e_pt;
            const bool is_inside_after  = s_pt <= new_point && new_point <= e_pt;

            if (is_inside_before xor is_inside_after)
                rpt->recount(v_itr, v_itr->id);

            sequential_search(dim, s_pt, &rpt->s_edge[dim], &rpt);
            sequential_search(dim, e_pt, &rpt->e_edge[dim], &rpt);
        }
    }

  public:
    container() { next_id = 0; }

    range_ptr_iterator new_range_pointer() {
        range_pointer rpt;
        for (unsigned int dim = 0; dim < DIM; dim++) {
            rpt.s_edge[dim] = table[dim].begin();
            rpt.e_edge[dim] = table[dim].begin();
            rpt.s_point[dim] = rpt.s_edge[dim]->point;
            rpt.e_point[dim] = rpt.e_edge[dim]->point;
        }

        range_ptr.emplace_back(rpt);

        return (--range_ptr.end());
    }

    template <typename I> void move_range(range_ptr_iterator rpt, I s_begin, I s_end, I e_begin, I e_end) {

        I s_itr = s_begin;
        I e_itr = e_begin;

        for (unsigned int dim = 0; dim < DIM; dim++) {

            const double s_pt = *s_itr <= *e_itr ? *s_itr : *e_itr;
            const double e_pt = *e_itr <= *s_itr ? *s_itr : *e_itr;

            rpt->s_point[dim] = s_pt;
            rpt->e_point[dim] = e_pt;

            sequential_search(dim, s_pt, &rpt->s_edge[dim], &rpt);
            sequential_search(dim, e_pt, &rpt->e_edge[dim], &rpt);

            s_itr++;
            e_itr++;
        }
    }

    void move_range(range_ptr_iterator rpt, std::initializer_list<double> s_coord, std::initializer_list<double> e_coord) {
        move_range(rpt, s_coord.begin(), s_coord.end(), e_coord.begin(), e_coord.end());
    }

    template <typename I> value_src_iterator insert(I begin, I end, T value) {

        I itr = begin;

        value_src.push_back(value_cont<cr_unit>(value, next_id));
        next_id++;
        value_src_iterator last_value_src_itr = --value_src.end();

        for (unsigned int dim = 0; dim < DIM; dim++) {

            table[dim].emplace_front(cr_unit{last_value_src_itr, *itr});
            last_value_src_itr->table_itr[dim] = table[dim].begin();
            arrange(dim, *itr, &last_value_src_itr->table_itr[dim]);

            itr++;
        }

        return last_value_src_itr;
    }
    
    value_src_iterator insert(std::initializer_list<double> coord, const T value) {
        return insert(coord.begin(), coord.end(), value);
    }

    void move_one(value_src_iterator value_src_itr, unsigned int dim, double new_point) {
        float old_point = value_src_itr->table_itr[dim]->point;
        value_src_itr->table_itr[dim]->point = new_point;
        arrange_for_move(dim, old_point, new_point, &value_src_itr->table_itr[dim]);
    }

    template <typename I> void move(const value_src_iterator value_src_itr, I begin, I end) {
        unsigned int dim = 0;
        for (I itr = begin; itr != end; itr++) {
            move_one(value_src_itr, dim, *itr);
            dim++;
        }
    }

    void move(const value_src_iterator value_src_itr, std::initializer_list<double> new_coord) {
        move(value_src_itr, new_coord.begin(), new_coord.end());
    }

    void change_value(const value_src_iterator value_src_itr, const T value) { value_src_itr->value = value; }

    void remove(const value_src_iterator value_src_itr) {

        for (range_ptr_iterator rpt = range_ptr.begin(); rpt != range_ptr.end(); rpt++) {

            for (unsigned int dim = 0; dim < DIM; dim++) {
                if (rpt->s_edge[dim] == value_src_itr->table_itr[dim]) {
                    rpt->s_edge[dim]++;
                }
                if (rpt->e_edge[dim] == value_src_itr->table_itr[dim]) {
                    rpt->e_edge[dim]++;
                }
            }

            rpt->level_info.erase(value_src_itr);
        }

        for (unsigned int dim = 0; dim < DIM; dim++) {
            table[dim].erase(value_src_itr->table_itr[dim]);
        }

        value_src.erase(value_src_itr);
    }
};

}; // namespace curtain_rail

template <typename T, unsigned int DIM>
using curtain_rail_index = typename curtain_rail::container<T, DIM>::value_src_iterator;
