
biome_chars = {
	


}

biome_types = {
	oceans = 1,
	plains = 2,
	hills = 3,
	mountains = 4,
	highlands = 5,
	plateau = 6,
	coast = 7,
}

biomes = {
	
	-- Plains
	grassy_plains = {
		name = "Grassy Plains", minTemp = -15, maxTemp = 95, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coast"], biome_types["plateau"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "light grass_green" },
		plants = { grass = 25, strawberry = 1, potato = 1, spinach = 1 },
		trees  = { deciduous = 30, evergreen = 5 }
	},

	-- Forests
	temperate_rainforest = {
		name = "Temperate Rainforest", minTemp = -20, maxTemp = 85, minRain = 65, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coast"] },
		soils  = { soil = 95, sand = 5 },
		rend   = { glyph = glyphs['ever_tree'], color = "dark forest_green" },
		plants = { grass = 20, blackberry = 2, plump_helmet = 2 },
		trees  = { deciduous = 10, evergreen = 55 }
	},

	-- Oceans
	sea_ice = {
		name = "Sea Ice", minTemp = -100, maxTemp = 5, minRain = 0, maxRain = 55,
		occurs = { biome_types["oceans"] },
		soils  = { soild = 0, sand = 100 },
		rend   = { glyph = glyphs['water'], color = "dark ice" },  
		plants = { none = 20, grass = 20 },
		trees  = { deciduous = 0, evergreen = 1}
	},

	-- Coasts
	
};