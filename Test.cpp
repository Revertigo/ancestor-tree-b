#include "doctest.h"
#include "FamilyTree.hpp"
using namespace family;

TEST_CASE("Check the correctness of additions"){
    Tree T("Apollo");
    CHECK(T.relation("Apollo") == "me");
    CHECK(T.find("me") == "Apollo");

    CHECK_NOTHROW(T.addFather("Apollo", "Zeus"));
    CHECK(T.relation("Zeus") == "father");
    CHECK(T.find("father") == "Zeus");

    CHECK_NOTHROW(T.addMother("Apollo", "Leto"));
    CHECK(T.relation("Leto") == "mother");
    CHECK(T.find("mother") == "Leto");

    CHECK_THROWS(T.addFather("Apollo", "Zeus1"));//It isn't ok to override the father
    CHECK_THROWS(T.addMother("Apollo", "Leto2"));//It isn't ok to override the mother
    CHECK_THROWS(T.addFather("a", "b"));
}

TEST_CASE("Check the correctness of relations"){
    Tree T("Yosef");
    T.addFather("Yosef", "Yaakov")
            .addMother("Yosef", "Rachel")
            .addFather("Yaakov", "Isaac")
            .addMother("Yaakov", "Rivka")
            .addFather("Isaac", "Avraham")
            .addFather("Avraham", "Terah");

    CHECK(T.relation("Yosef") == "me");
    CHECK(T.relation("Yaakov") == "father");
    CHECK(T.relation("Rachel") == "mother");
    CHECK(T.relation("Isaac") == "grandfather");
    CHECK(T.relation("Rivka") == "grandmother");
    CHECK(T.relation("Avraham") == "great-grandfather");
    CHECK(T.relation("Terah") == "great-great-grandfather");

    CHECK(T.relation("Tera") == "unrelated");
    CHECK(T.relation("Yahweh") == "unrelated");
}

TEST_CASE("Check the correctness of find"){
    Tree T("Yosef");
    T.addFather("Yosef", "Yaakov")
            .addMother("Yosef", "Rachel")
            .addFather("Yaakov", "Isaac")
            .addMother("Yaakov", "Rivka")
            .addFather("Isaac", "Avraham")
            .addFather("Avraham", "Terah");

    CHECK(T.find("me") == "Yosef");
    CHECK(T.find("father") == "Yaakov");
    CHECK(T.find("mother") == "Rachel");
    CHECK(T.find("grandfather") == "Isaac");
    CHECK(T.find("grandmother") == "Rivka");
    CHECK(T.find("great-grandfather") == "Avraham");
    CHECK(T.find("great-great-grandfather") == "Terah");

    CHECK_THROWS(T.find("great-great-gr-grandfather"));//None valid input
    CHECK_THROWS(T.find("great-great-grandmother"));//Valid input, none exists relation
    CHECK_THROWS(T.find("uncle"));
}

TEST_CASE("Check the correctness of remove 1") {
    Tree T("Judah");
    T.addFather("Judah", "Yaakov");

    CHECK(T.relation("Judah") == "me");
    CHECK(T.relation("Yaakov") == "father");

    CHECK(T.find("me") == "Judah");
    CHECK(T.find("father") == "Yaakov");

    T.remove("Yaakov");
    CHECK(T.relation("Judah") == "me");
    CHECK(T.find("me") == "Judah");

    CHECK(T.relation("Yaakov") == "unrelated");
    CHECK_THROWS(T.find("father"));

    T.addMother("Judah", "Leha");

    CHECK(T.relation("Judah") == "me");
    CHECK(T.relation("Yaakov") == "unrelated");

    CHECK(T.find("me") == "Judah");
    CHECK_THROWS(T.find("father"));

    CHECK_THROWS(T.remove("Judah"));//Root of the tree, should throw exception

    CHECK(T.relation("Judah") == "me");
    CHECK(T.find("me") == "Judah");

    CHECK(T.relation("Leha") == "mother");
    CHECK(T.find("mother") == "Leha");
}

TEST_CASE("Check the correctness of remove 2") {
    Tree T("Yosef");
    T.addFather("Yosef", "Yaakov")
            .addMother("Yosef", "Rachel")
            .addFather("Yaakov", "Isaac")
            .addMother("Yaakov", "Rivka")
            .addFather("Isaac", "Avraham")
            .addFather("Avraham", "Terah");

    T.remove("Yaakov");

    //Check all of them removed
    CHECK(T.relation("Yaakov") == "unrelated");
    CHECK_THROWS(T.find("father"));

    CHECK(T.relation("Rivka") == "unrelated");
    CHECK_THROWS(T.find("grandmother"));

    CHECK(T.relation("Isaac") == "unrelated");
    CHECK_THROWS(T.find("grandfather"));

    CHECK(T.relation("Avraham") == "unrelated");
    CHECK_THROWS(T.find("great-grandfather"));

    CHECK(T.relation("Avraham") == "unrelated");
    CHECK_THROWS(T.find("great-grandfather"));

    CHECK(T.relation("Terah") == "unrelated");
    CHECK_THROWS(T.find("great-great-grandfather"));

    //Check all the rest exists
    CHECK(T.relation("Rachel") == "mother");
    CHECK(T.find("mother") == "Rachel");

    CHECK(T.relation("Yosef") == "me");
    CHECK(T.find("me") == "Yosef");
}

TEST_CASE("Check the correctness of remove 3") {
    Tree T("Judah");
    T.addFather("Judah", "Yaakov")
            .addMother("Judah", "Leha")
            .addFather("Yaakov", "Isaac")
            .addMother("Yaakov", "Rivka")
            .addFather("Isaac", "Avraham")
            .addMother("Isaac", "Sarah")
            .addFather("Avraham", "Terah");

    CHECK_THROWS(T.remove("Yahweh"));//None existing nodes
    CHECK_THROWS(T.remove("Yosef"));//None existing nodes

    //Remove the whole tree - root of the tree, should throw exception
    CHECK_THROWS(T.remove("Judah"));

    //Check all of family still exists
    CHECK(T.relation("Judah") == "me");
    CHECK(T.find("me") == "Judah");

    CHECK(T.relation("Yaakov") == "father");
    CHECK(T.find("father") == "Yaakov");

    CHECK(T.relation("Leha") == "mother");
    CHECK(T.find("mother") == "Leha");

    CHECK(T.relation("Rivka") == "grandmother");
    CHECK(T.find("grandmother") == "Rivka");

    CHECK(T.relation("Isaac") == "grandfather");
    CHECK(T.find("grandfather") == "Isaac");

    CHECK(T.relation("Avraham") == "great-grandfather");
    CHECK(T.find("great-grandfather") == "Avraham");

    CHECK(T.relation("Sarah") == "great-grandmother");
    CHECK(T.find("great-grandmother") == "Sarah");

    CHECK(T.relation("Terah") == "great-great-grandfather");
    CHECK(T.find("great-great-grandfather") == "Terah");
}

TEST_CASE("Check the correctness of whole API") {
    Tree T1("Rehoboam");
    Tree T2("Judah");

    CHECK_THROWS(T1.remove("Yahweh"));//Non existing node
    CHECK_THROWS(T2.remove("Yahweh"));//Non existing node

    CHECK_THROWS(T1.remove("Rehoboam"));//Root of the tree
    CHECK(T2.relation("Judah") == "me");
    CHECK(T2.find("me") == "Judah");

    //Check single relation addition and remove of father
    CHECK_NOTHROW(T1.addFather("Rehoboam", "Solomon"));
    CHECK(T1.relation("Solomon") == "father");
    CHECK(T1.find("father") == "Solomon");

    CHECK_NOTHROW(T1.remove("Solomon"));

    CHECK(T1.relation("Yaakov") == "unrelated");
    CHECK_THROWS(T1.find("father"));

    //Check single relation addition and remove of mother
    CHECK_NOTHROW(T1.addMother("Rehoboam", "Naamah"));
    CHECK(T1.relation("Naamah") == "mother");
    CHECK(T1.find("mother") == "Naamah");

    CHECK_NOTHROW(T1.remove("Naamah"));

    CHECK(T1.relation("Naamah") == "unrelated");
    CHECK_THROWS(T1.find("mother"));

    //Build up the whole tree, and check every step
    T1.addFather("Rehoboam", "Solomon");
    CHECK(T1.relation("Solomon") == "father");
    CHECK(T1.find("father") == "Solomon");

    T1.addMother("Rehoboam", "Naamah");
    CHECK(T1.relation("Naamah") == "mother");
    CHECK(T1.find("mother") == "Naamah");

    T1.addFather("Solomon", "David");
    CHECK(T1.relation("David") == "grandfather");
    CHECK(T1.find("grandfather") == "David");

    T1.addMother("Solomon", "Bathsheba");
    CHECK(T1.relation("Bathsheba") == "grandmother");
    CHECK(T1.find("grandmother") == "Bathsheba");

    T1.addFather("David", "Jesse");
    CHECK(T1.relation("Jesse") == "great-grandfather");
    CHECK(T1.find("great-grandfather") == "Jesse");

    //Few more checks
    CHECK(T1.relation("Yosef") != "me");
    CHECK(T1.relation("Rehoboam") == "me");
    CHECK(T1.find("me") == "Rehoboam");
    CHECK_THROWS(T1.find("great-great-grandfather"));

    //Check remove node, then add it again, then remove it again
    CHECK_NOTHROW(T1.remove("Jesse"));
    CHECK(T1.relation("Jesse") == "unrelated");
    CHECK_THROWS(T1.find("great-grandfather"));

    T1.addFather("David", "Jesse");
    CHECK(T1.relation("Jesse") == "great-grandfather");
    CHECK(T1.find("great-grandfather") == "Jesse");

    CHECK_NOTHROW(T1.remove("Jesse"));
    CHECK_THROWS(T1.remove("Jesse"));
    CHECK(T1.relation("Jesse") == "unrelated");
    CHECK_THROWS(T1.find("great-grandfather"));

    //Now let's check the rest of the tree is valid
    CHECK(T1.relation("Rehoboam") == "me");
    CHECK(T1.relation("Solomon") == "father");
    CHECK(T1.relation("Naamah") == "mother");
    CHECK(T1.relation("David") == "grandfather");
    CHECK(T1.relation("Bathsheba") == "grandmother");

    CHECK(T1.find("me") == "Rehoboam");
    CHECK(T1.find("father") == "Solomon");
    CHECK(T1.find("mother") == "Naamah");
    CHECK(T1.find("grandfather") == "David");
    CHECK(T1.find("grandmother") == "Bathsheba");

    CHECK_NOTHROW(T1.display());//call to display should always success

    //Check remove one by one
    CHECK_NOTHROW(T1.remove("David"));
    CHECK(T1.relation("David") == "unrelated");
    CHECK_THROWS(T1.find("grandfather"));

    CHECK_THROWS(T1.remove(" Bathsheba "));//" Bathsheba " should not be exists
    CHECK_NOTHROW(T1.remove("Bathsheba"));
    CHECK(T1.relation("Bathsheba") == "unrelated");
    CHECK_THROWS(T1.find("grandmother"));

    CHECK_NOTHROW(T1.remove("Solomon"));
    CHECK(T1.relation("Solomon") == "unrelated");
    CHECK_THROWS(T1.find("father"));

    CHECK_NOTHROW(T1.remove("Naamah"));
    CHECK(T1.relation("Naamah") == "unrelated");
    CHECK_THROWS(T1.find("mother"));

    CHECK_THROWS(T1.remove(" Rehoboam"));//Root of the tree, throws exception
    CHECK_THROWS(T1.remove("Rehoboam"));//Root of the tree, throws exception
    CHECK(T1.relation("Rehoboam") == "me");
    CHECK(T1.find("me") == "Rehoboam");

    CHECK_NOTHROW(T1.display());//call to display should always success

    //Check add for none existing nodes
    CHECK_THROWS(T2.addFather("a", "b"));
    CHECK_THROWS(T2.addMother("b", "c"));
    CHECK_THROWS(T1.addFather("a", "B"));
    CHECK_THROWS(T1.addMother("a", "b"));

    T2.addFather("Judah", "a")
        .addFather("a", "b")
        .addFather("b", "c")
        .addFather("c", "d")
        .addFather("d", "e")
        .addFather("e", "f");

    CHECK(T2.relation("f") == "great-great-great-great-grandfather");
    CHECK(T2.find("great-great-great-great-grandfather") == "f");
}

TEST_CASE("Test case 1") {
    Tree T("Yosef");
    T.addMother("Yosef","Sarah")
            .addFather("Yosef","Adam")
            .addFather("Sarah","Adam")
            .addMother("Adam","Ronit")
            .addFather("Ronit","Shmuel")
            .addFather("Adam", "Tom")
            .addMother("Sarah", "Revital");
    CHECK_THROWS(T.addMother("Adam", "Zeava"));

            T.display();
            CHECK(T.relation("Yosef") == string("me"));
            CHECK(T.relation("Sarah") == string("mother"));
            CHECK(T.relation("Adam") == string("father"));
            CHECK(T.relation("Revital") == string("grandmother"));
            CHECK(T.relation("Ronit") == string("grandmother"));
            CHECK(T.relation("Tom") == string("grandfather"));
            CHECK(T.relation("Zeava") == string("unrelated"));
            CHECK(T.relation("Shmuel") == string("great-grandfather"));
            CHECK(T.relation("Dani") == string("unrelated"));
            CHECK(T.relation("Zeev") == string("unrelated"));
            CHECK(T.relation("abcd") == string("unrelated"));

            CHECK(T.find("mother") == string("Sarah"));
            CHECK(T.find("father") == string("Adam"));
            CHECK(T.find("grandfather") == string("Tom"));
            CHECK_THROWS(T.find("great-grandmother"));
            CHECK(T.find("great-grandfather") == string("Shmuel"));
            CHECK_THROWS(T.find("great-great-great-grandmother"));
}

TEST_CASE("Test case 2") {
    Tree T("b");
    T.addMother("b","c")
            .addFather("c","d")
            .addMother("d","e")
            .addFather("e","f")
            .addMother("f", "g")
            .addFather("g","h")
            .addMother("h", "i")
            .addFather("i", "j")
            .addMother("j","k")
            .addFather("k","l");

            CHECK(T.relation("b") == string("me"));
            CHECK(T.relation("c") == string("mother"));
            CHECK(T.relation("d") == string("grandfather"));
            CHECK(T.relation("e") == string("great-grandmother"));
            CHECK(T.relation("f") == string("great-great-grandfather"));
            CHECK(T.relation("g") == string("great-great-great-grandmother"));
            CHECK(T.relation("h") == string("great-great-great-great-grandfather"));
            CHECK(T.relation("i") == string("great-great-great-great-great-grandmother"));
            CHECK(T.relation("j") == string("great-great-great-great-great-great-grandfather"));
            CHECK(T.relation("k") == string("great-great-great-great-great-great-great-grandmother"));
            CHECK(T.relation("l") == string("great-great-great-great-great-great-great-great-grandfather"));
            CHECK(T.relation("m") == string("unrelated"));

            CHECK(T.find("me") == string("b"));
            CHECK(T.find("mother") == string("c"));
            CHECK(T.find("grandfather") == string("d"));
            CHECK(T.find("great-grandmother") == string("e"));
            CHECK(T.find("great-great-grandfather") == string("f"));
            CHECK(T.find("great-great-great-grandmother") == string("g"));
            CHECK(T.find("great-great-great-great-grandfather") == string("h"));
            CHECK(T.find("great-great-great-great-great-grandmother") == string("i"));
            CHECK(T.find("great-great-great-great-great-great-grandfather") == string("j"));
            CHECK(T.find("great-great-great-great-great-great-great-grandmother") == string("k"));
            CHECK(T.find("great-great-great-great-great-great-great-great-grandfather") == string("l"));

            CHECK_THROWS(T.find("father"));
            CHECK_THROWS(T.find("grandmother") );
            CHECK_THROWS(T.find("great-grandfather") );
            CHECK_THROWS(T.find("great-great-grandmother") );
            CHECK_THROWS(T.find("great-great-great-grandfather") );
            CHECK_THROWS(T.find("great-great-great-great-grandmother")  );
            CHECK_THROWS(T.find("great-great-great-great-great-grandfather") );
            CHECK_THROWS(T.find("great-great-great-great-great-great-grandmother") );
            CHECK_THROWS(T.find("great-great-great-great-great-great-great-grandfather") );
            CHECK_THROWS(T.find("great-great-great-great-great-great-great-great-grandmother") );
            CHECK_THROWS(T.find("uncle"));

    CHECK_THROWS(T.remove("b"));

            CHECK_NOTHROW(T.find("me") );
            CHECK_NOTHROW(T.find("mother"));
            CHECK_NOTHROW(T.find("grandfather"));
            CHECK_NOTHROW(T.find("great-grandmother") );
            CHECK_NOTHROW(T.find("great-great-grandfather"));
            CHECK_NOTHROW(T.find("great-great-great-grandmother"));
            CHECK_NOTHROW(T.find("great-great-great-great-grandfather") );
            CHECK_NOTHROW(T.find("great-great-great-great-great-grandmother") );
            CHECK_NOTHROW(T.find("great-great-great-great-great-great-grandfather") );
            CHECK_NOTHROW(T.find("great-great-great-great-great-great-great-grandmother"));
            CHECK_NOTHROW(T.find("great-great-great-great-great-great-great-great-grandfather") );

//            CHECK(T.relation("b") == string("unrelated"));
//            CHECK(T.relation("c") == string("unrelated"));
//            CHECK(T.relation("d") == string("unrelated"));
//            CHECK(T.relation("e") == string("unrelated"));
//            CHECK(T.relation("f") == string("unrelated"));
//            CHECK(T.relation("g") == string("unrelated"));
//            CHECK(T.relation("h") == string("unrelated"));
//            CHECK(T.relation("i") == string("unrelated"));
//            CHECK(T.relation("j") == string("unrelated"));
//            CHECK(T.relation("k") == string("unrelated"));
//            CHECK(T.relation("l") == string("unrelated"));
//            CHECK(T.relation("z") == string("unrelated"));
//            CHECK(T.relation("n") == string("unrelated"));
//            CHECK(T.relation("o") == string("unrelated"));

}

TEST_CASE("Test case 3") {
    Tree T("a");
    T.addMother("a","b")
            .addFather("a","a")
            .addMother("b","c")
            .addFather("c","a")
            .addMother("c", "d")
            .addFather("d","a")
            .addMother("d", "b")
            .addFather("b", "a");

            CHECK(T.relation("a") == string("me"));
            CHECK(T.relation("b") == string("mother"));
            CHECK(T.relation("c") == string("grandmother"));
            CHECK(T.relation("d") == string("great-grandmother"));
            CHECK(T.relation("e") == string("unrelated"));

            CHECK(T.find("me") == string("a"));
            CHECK(T.find("father") == string("a"));
            CHECK(T.find("grandfather") == string("a"));
            CHECK(T.find("great-grandfather") == string("a"));
            CHECK(T.find("great-great-grandfather") == string("a"));
            CHECK(T.find("mother") == string("b"));
            CHECK(T.find("grandmother") == string("c"));
            CHECK(T.find("great-grandmother") == string("d"));
            CHECK(T.find("great-great-grandmother") == string("b"));

    T.remove("d");
            CHECK_THROWS(T.find("great-great-grandfather"));
            CHECK_THROWS(T.find("great-great-grandmother"));
            CHECK_THROWS(T.find("great-grandmother"));

    T.addMother("c","x")
            .addMother("x","y")
            .addFather("y","z");

            CHECK(T.find("great-grandmother") == string("x"));
            CHECK(T.find("great-great-grandmother") == string("y"));
            CHECK(T.find("great-great-great-grandfather") == string("z"));

    T.remove("b");

            CHECK(T.relation("b") == string("unrelated"));
            CHECK(T.relation("c") == string("unrelated"));
            CHECK(T.relation("d") == string("unrelated"));

    T.addMother("a","d");

            CHECK_THROWS(T.find("grandfather"));
            CHECK_THROWS(T.find("grandmother") );
            CHECK_THROWS(T.find("great-grandfather"));
            CHECK_THROWS(T.find("great-grandmother"));

            CHECK(T.relation("x") == string("unrelated"));
            CHECK(T.relation("y") == string("unrelated"));
            CHECK(T.relation("z") == string("unrelated"));
            CHECK(T.relation("w") == string("unrelated"));

}