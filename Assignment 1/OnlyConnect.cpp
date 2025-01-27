/* File: OnlyConnect.cpp
 *
 * TODO: Edit these comments to describe anything interesting or noteworthy in your implementation.
 *
 * TODO: Edit these comments to leave a puzzle for your section leader to solve!
 */
#include "OnlyConnect.h"
#include "strlib.h"
using namespace std;

string onlyConnectize(string phrase) {
    /* checks base case if the phrase has no more letters then return empty phrase */
    if (phrase.length() == 0){
        return phrase;
    }
    /*Creates a variable that stores the first character of phrase*/
    char firstChar = phrase[0];
    /*checs if the first character is even a letter */
    if (isalpha(firstChar) != 0){
        /*creates a string with uppercase and lowercase vowels*/
        string vowels = "aeiouAEIOU";
        /* checks if first letter is a vowel */
        if (stringContains(vowels, firstChar)){
            /*if it is then call onlyConectize without phrase's first letter */
            return onlyConnectize(phrase.substr(1));
        }
        else {
            /*if it isn't then keep the consanent and capitalize it and return it with onlyConnectize without phrase's first letter */
            return toUpperCase(firstChar) + onlyConnectize(phrase.substr(1));
        }
    }
    else{
        return onlyConnectize(phrase.substr(1));
    }

}






/* * * * * * Provided Test Cases * * * * * */
#include "GUI/SimpleTest.h"

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

/* TODO: You will need to add your own tests into this suite of test cases. Think about the sorts
 * of inputs we tested here, and, importantly, what sorts of inputs we *didn't* test here. Some
 * general rules of testing:
 *
 *    1. Try extreme cases. What are some very large cases to check? What are some very small cases?
 *
 *    2. Be diverse. There are a lot of possible inputs out there. Make sure you have tests that account
 *       for cases that aren't just variations of one another.
 *
 *    3. Be sneaky. Don't just try standard inputs. Try weird ones that you wouldn't expect anyone to
 *       actually enter, but which are still perfectly legal.
 *
 * Happy testing!
 */

PROVIDED_TEST("Handles non-letter inputs.") {
    EXPECT_EQUAL(onlyConnectize("l0wercas3"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("upp3rc4se"), "PPRCS");
}




