local g = "grass"
local w = "water"
local a = "wall"
local d = "dirt"

local set = {g, w, d}

local tiles = {}
for y=0,49 do
    local row = {}
    tiles[y] = row
    for x=0,49 do
        row[x] = set[math.random(1, 3)]
    end
end

data = {
    name = "map1",
    tiles = tiles
}
