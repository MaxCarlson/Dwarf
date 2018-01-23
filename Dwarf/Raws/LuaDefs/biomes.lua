
biome_chars = {
	


}

biome_types = {
	plains = 1,
	hills = 2,
	mountains = 3,
	plateau = 4,
	oceans = 5,
	coast = 6
}

biomes = {
	
	-- Plains
	grassy_plains = {
		name = "Grassy Plains", minTemp = -15, maxTemp = 95, minRain = 15, maxRain = 100,
		occurs = { biome_types["plains"], biome_types["coast"], biome_types["plateau"] },
		soils  = { soil = 85, sand = 15 },
		rend   = { glyph = glyphs['grass'], color = "light grass_green" },
		plants = { grass = 25 },
		trees  = { deciduous = 30, evergreen = 5 }
	},

	-- Forests
	temperate_rainforest = {
		name = "Temperate Rainforest", minTemp = -20, maxTemp = 85, minRain = 65, maxRain = 100,
		occurs = { biome_types["hills"], biome_types["mountains"], biome_types["coast"] },
		soils  = { soil = 95, sand = 5 },
		rend   = { glyph = glyphs['ever_tree'], color = "forest_green" },
		plants = { grass = 20 },
		trees  = { deciduous = 10, evergreen = 55 }
	},

};