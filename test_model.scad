//test object with overhang of different heights

height_of_base = 20;
width_of_base = 5;
length_of_base = 10;
epsilon = 0.01;

module base()
{
    translate([0, 0, height_of_base/2-epsilon])
    cube([length_of_base, width_of_base, height_of_base], center = true);
}
//base()

module top()
{
    translate([0, 0, height_of_base-epsilon])
    cube([height_of_base*1.5, length_of_base, width_of_base], center = true);
}
//top()

module spout()
{
    translate([height_of_base*1.5/2-width_of_base/2, 0, height_of_base-width_of_base+epsilon])
    cube([width_of_base, length_of_base, width_of_base], center = true);
}
//spout()

module faucet()
{
    union()
    {
        base();
        top();
        spout();
    }
}

faucet();
