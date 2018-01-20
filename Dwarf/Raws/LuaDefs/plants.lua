
grass_lifecycle = { 10, 28, 90, 90, 3 };
fast_lifecycle  = { 10, 28, 60, 60, 3 };
slow_lifecycle  = { 28, 40, 90, 90, 0 };

function build_plants(name, cycles, glyphs, harvest, tags) return { name = name, cycles = cycles, glyphs = glyphs, harvest = harvest, tags = tags; } end


function veg_g(a, b, c, d) return { glyphs[a], glyphs[b], glyphs[c], glyphs[d] } end

function veg_a(a, ac, b, bc, c, cc, d, dc) return 
{
	a = { glyphs[a], col = ac }, 
	b = { glyphs[b], col = bc }, 
	c = { glyphs[c], col = cc }, 
	d = { glyphs[d], col = dc }
} end


grass_glyphs  = {'grass', 'brown',  'grass', 'green', 'grass', 'brown', 'grass', 'brown' };
flower_glyphs = {'grass', 'brown', 'sprout', 'green', 'flower', 'green', 'sprout', 'yellow' };

vegitation = 
{
	grass = 
}