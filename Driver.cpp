#include"jsonParser.hpp"
#include"Algorithm.cpp"

main(){

    JSONParser jp;
    jp.parseFile();
    jp.deserialize();

    Algorithm algorithm(jp.rooms, jp.courses, jp.teachers, jp.sections, jp.lectures);
    algorithm.initialize();
    algorithm.run();

}