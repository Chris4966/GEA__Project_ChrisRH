var Name = "Player 1";
var Level = 1;
var Health = 100.0;
var Strength = 10.0;
var Speed = 1.5;

function takeDamage(str, hp)
{
    return hp - str;
}

function levelUp(lvl)
{
    return lvl + 1;
}
