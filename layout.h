/*
    Mingfei Jiang
    1320376
    COMPSCI 3GC3 Final Project
    Magic Tower
*/

#include <string>
#include <vector>
using namespace std;

// . wall
//   floor
// y yellowDoor
// Y yellowKey
// b blueDoor
// B blueKey
// r redDoor
// R redKey
// ! smallPotion
// @ mediumPotion
// # largePotion
// d blueGem (def)
// a redGem (att)
// s sword

// define the layout of the floors using strings
vector< vector<string> > floors = {{
    ".............",
    ".343.. ..252.",
    ". . .   . . .",
    ".Y.y..1..y.Y.",
    ". . 2   2 . .",
    ".d...Y!Y...a.",
    "...1y!R!y1...",
    ".Y@!.Y!Y.!@Y.",
    "......r......",
    ".Y4!.   .@5B.",
    ".B46y2.3y75R.",
    ".Y4!. U .@5B.",
    ".............",
},
{
    ".............",
    ".U  .@!@.  Y.",
    "... . . . ...",
    ".!.5.1 1.4.@.",
    ".3. ..y.. .6.",
    ". . .Y Y. . .",
    ".!. ..4.. .@.",
    ".2.       .5.",
    ". . .. .. . .",
    ".!.2.   .1.@.",
    ".1. . . . .4.",
    ".  @. D .!  .",
    ".............",
},
{
    ".............",
    ".D Y Y Y Y U.",
    ".............",
    ". .       . .",
    ".  !.. ..!  .",
    ".a. .@ @. .d.",
    "...@.a d.@...",
    ".!  .....  !.",
    ". .       . .",
    ". ....a.... .",
    ". . a. .d . .",
    ".   B.d.B   .",
    ".............",
},
{
    ".............",
    ".U .Y. . . D.",
    ".  .Y. .3.1..",
    ".. .B. y .y .",
    ".. .5. .1   .",
    ".. y . ......",
    ".. . .1   4 .",
    ".. . ....y...",
    ".. .1       .",
    ".. ..........",
    "..1 5 6 6 6R.",
    ".s .@.Y.Y.Y..",
    ".............",
},
{
    ".............",
    ".D 1.Y 2.  3.",
    ". 3 y 3 y 4 .",
    ".1 !.2 !.3  .",
    "..b...b...y..",
    ".  2.  3.Y 4.",
    ". 3 y 4 y 5 .",
    ".2 !.3  .4 !.",
    "..y...b...b..",
    ".  3.Y 4.  5.",
    ". 4 . 5 . 6 .",
    ".3  .4 !.5 U.",
    ".............",
},
{
    ".............",
    ".Y .U.! !.4a.",
    ". 3y ..6.. 4.",
    ".Y .     3  .",
    "..... . .....",
    ".! !.3.5.!! .",
    ". B .4. 4 4 .",
    ".Y6Y.3.Y .  .",
    ".6 6. ....y..",
    ". 7 .  d.   .",
    ".. ...b.. 6 .",
    ".!  2      D.",
    ".............",
},
{
    ".............",
    ".6S.D. 7@..a.",
    ". .. 5@. 7yd.",
    ".5!.5........",
    ".. . .U.   !.",
    ".44. .3.y.. .",
    ". ..Y. .2!. .",
    ".Y5.Y.3.!2.!.",
    "..y.Y. ..y. .",
    ". !. .! 6 .7.",
    ".y..6...... .",
    ".33y  3 Y  !.",
    ".............",
},
{
    ".............",
    "....     ....",
    ".. .     7 ..",
    "..7. ... ....",
    ".    .D.6   .",
    ". .7.. . ....",
    ". . .    . ..",
    ". ...... .7..",
    ".  6 7 .6   .",
    ".... ...... .",
    ".. .    . 7 .",
    ".. .    ...U.",
    ".............",
},
{
    ".............",
    ".           .",
    ". ....b.... .",
    ".7.... ....7.",
    ". ....4.... .",
    ". .... .... .",
    ". ....b.... .",
    ". .... .... .",
    ". ....6.... .",
    ". .... .... .",
    ". ....r.... .",
    ".B....U....D.",
    ".............",
},
{
    ".............",
    ".............",
    ".............",
    ".............",
    "....     ....",
    "....     ....",
    "....  8  ....",
    "....     ....",
    "....     ....",
    "......r......",
    "...... ......",
    "......D......",
    ".............",
},
};