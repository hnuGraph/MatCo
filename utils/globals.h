#ifndef UTILS_GLOBALS
#define UTILS_GLOBALS

#include <atomic>
#include <vector>

extern std::atomic<bool> reach_time_limit;

extern size_t batches_num;

extern size_t empty_set_num;

extern size_t full_coverage_test_num;

extern size_t partial_match_is_all_kv_num;

extern size_t full_coverage_suc_num;

extern size_t candidate_set_test_num;

extern size_t candidate_set_flag_reuse_num;

extern size_t muti_exp_num;




#endif //UTILS_GLOBALS
