#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT test: public contract {
    public:
        using contract::contract;
        ACTION hi(name user, std::string first, std::string last_name, uint32_t age){
           require_auth(user);
           test_index forUpsert(get_self(), get_self().value);
           auto itr = forUpsert.find(user.value); 
        }

}        //hi : allowance에 등록된 계정만 이 액션을 실행할 수 있다.

        //delete : allowance에 계정 삭제, 컨트랙트 배포자만 실행할 수 있도록
        //insert : allowance에 계정 등록, 컨트랙트 배포자만 실행할 수 있도록
        ACTION insert(name user, std::string first_name, std::string last_name, uint32_t age) {
            require_auth(user);

            test_index forInsert(get_self(), get_self().value);
            auto itr = forInsert.find(user.value);

            check(itr == forInsert.end(), "already exists");

            forInsert.emplace(user, [&](auto & row) {
                row.user = user;
                row.first_name = first_name;
                row.last_name = last_name;
                row.age = age;
            });

            print("insert success");
        }

        ACTION erase(name user) {
            require_auth(user);

            address_index forerase(get_self(), get_self().value);
            auto itr = forErase.require_find(user.value, "no account");
            forErase.delete(itr);

            print("delete success");
        }
    private:
        struct [ [eosio::table] ] person {
            name user;
            std:: string first_name;
            std:: string last_name;
            uint32_t age;

            uint64_t primary_key() const { return user.value; }
        };

        typedef multi_index<"people"_n, person> address_index;

};