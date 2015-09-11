local set = {"grass", "water", "dirt"}

local tiles = {}
for y=0,19 do
    local row = {}
    tiles[y] = row
    for x=0,19 do
        row[x] = set[math.random(1, 3)]
    end
end

data = {
    name = "map1",
    tiles = tiles
}
