
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
		rend   = { glyph = glyphs['grass'], color = "grass_green" },
		plants = { grass = 25 },
		trees  = { deciduous = 30, evergreen = 5 }
	},

};