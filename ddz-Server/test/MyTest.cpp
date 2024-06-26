//
// Created by Hasee on 2024/4/13.
//

#include "MyTest.h"
#include "Person.pb.h"
void MyTest::test() {
    Person p;
    p.set_id(10);
    p.set_age(32);
    p.set_sex("man");
    p.set_name("lucy");
    std::string output;
    p.SerializeToString(&output);

    Person pp;
    pp.ParseFromString(output);
    std::cout<<pp.id()<<","<<pp.sex()<<","<<pp.name()<<","<<pp.age()<<","<<std::endl;

}
