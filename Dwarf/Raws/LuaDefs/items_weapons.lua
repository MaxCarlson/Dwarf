

-- Melee weapons
items["short_sword"] = { -- 3.6 dps
    name = "Short Sword",
	description = "A fast sword",
	itemType = {"melee"},
	glyph = glyphs["short_sword"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["weapons"].id,
	baseDmg = 2.4,
	baseSpeed = 1.5
}

items["bastard_sword"] = { -- 3.705 dps
    name = "Bastard Sword",
	description = "A very balanced weapon",
	itemType = {"melee"},
	glyph = glyphs["sword"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["weapons"].id,
	baseDmg = 2.85,
	baseSpeed = 1.3
}

items["long_sword"] = { -- 6.1 dps
    name = "Long Sword",
	description = "A large sword with a long reach",
	itemType = {"melee"},
	glyph = glyphs["long_sword"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["weapons"].id,
	baseDmg = 6.1,
	baseSpeed = 1.0,
	twoHanded = true,
}

items["axe"] = { -- 3.84 dps
    name = "A medium size axe",
	description = "It fits just right.",
	itemType = {"melee"},
	glyph = glyphs["axe"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["weapons"].id,
	baseDmg = 4.0,
	baseSpeed = 0.96
}

items["battle_axe"] = { -- 6.175 dps
    name = "Battle Axe",
	description = "An intimidatingly large axe",
	itemType = {"melee"},
	glyph = glyphs["large_axe"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["weapons"].id,
	baseDmg = 9.5,
	baseSpeed = 0.65, 
	twoHanded = true
}