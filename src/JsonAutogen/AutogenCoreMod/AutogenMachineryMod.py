import json

machineTypes = {"electric furnance", "macerator", "plate bending machine", "magnetic separator", "chemical reactor"}

generatorTypes = {"diesel engine", "steam turbine", "gas turbine", "automated furnance"}
generatorTiers = {"primitive", "basic", "advanced", "ultimate"}

beginId = 200;
id = beginId;

data = []
for generator in generatorTypes:
    for tier in generatorTiers:
        name = tier + " " + generator 
        item = { 
           "section": "block",
            "name": name,
            "id": id,
            "tesselator": {
              "type": "TesselatorSolidBlock",
              "tex": ["data/furnance_front.png",
                "data/furnance_side.png",
                "data/furnance_side.png",
                "data/furnance_side.png",
                "data/furnance_top_bottom.png",
                "data/furnance_top_bottom.png"]
            },
            "type":"BlockAutoCrafter",
            "agents":[{
                "type":"AccessorItem",
	        	"name":"0",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"1",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"2",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"3",
                "size":4
              }],
            "Crafter1":
            {
              "Recipe":"RecipeHand",
              "fast":False,
              "Input":"1",
              "Output":"0"
            },
            "Crafter2":
            {
              "Recipe":"RecipeBurn",
              "fast":True,
              "Input":"2",
              "Output":"2"
            },
            "generate_item":{
              "tex":"data/items/furnance_front.png",
              "name" : name
            },
            "drop":[[name, 1]]
        }
        data.append(item)
        id += 1

with open('AutogenMachineryMod.json', 'w') as outfile:
    json.dump(data, outfile, indent=4, sort_keys=True)

print str(id - beginId) + " objects is done"