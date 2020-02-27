#include <vector>
#include "character.hpp"
#include "asset_manager.hpp"
#include "json_handler.hpp"
#include "frame_data.hpp"

using JSON = nlohmann::json;

Character::Character()
{
    towardHSpeed = 10;
    backHSpeed = 6;
    accellerationH = 4;
    JSON json = JsonHandler::loadJsonFile("assets/sample.json");

    moveList.movesSize = json.size();
    moveList.moves = new Move[moveList.movesSize];

    for (int i = 0; i < moveList.movesSize; i++)
    {
        moveList.moves[i].name = json[i]["name"].get<std::string>();
        moveList.moves[i].actionId = json[i]["actionId"].get<std::string>();
        moveList.moves[i].triggerBtn = (PlayerInput)json[i]["triggerBtn"].get<int>();
        moveList.moves[i].commandSequenceSize = json[i]["commandSequence"].size();
        for (int i = 0; i < moveList.moves[i].commandSequenceSize; i++)
        {
            moveList.moves[i].commandSequence[i] = (PlayerInput)json[i]["commandSequence"][i].get<int>();
        }
        moveList.moves[i].frameData.frameDataFromJSON(json[i]["frameData"]);
    }

    model = AssetManager::addModel("assets/models/characters/d-func");
    // model = new ModelController;
    model->init("assets/models/characters/d-func");
}
Character::~Character()
{
    unload();
}

void Character::render()
{
    if (*playerData != NULL)
    {
        _convertTranslation();

        model->render((*playerData)->transform.translation, (*playerData)->actionFace);
    }

    if (playerBoxes != NULL && *playerBoxes != NULL)
    {
        PlayerBoxes *box = *playerBoxes;
        for (int i = 0; i < box->pushBoxSize; i++)
        {
            _renderBox(box->pushBoxArray[i]);
        }
        for (int i = 0; i < box->grabBoxesSize; i++)
        {
            _renderBox(box->grabBoxesArray[i]);
        }
        for (int i = 0; i < box->hitBoxesSize; i++)
        {
            _renderBox(box->hitBoxesArray[i]);
        }
        for (int i = 0; i < box->hurtBoxesSize; i++)
        {
            _renderBox(box->hurtBoxesArray[i]);
        }
        for (int i = 0; i < box->proximityBoxesSize; i++)
        {
            _renderBox(box->proximityBoxesArray[i]);
        }
    }
}

void Character::unload()
{}

void Character::_convertTranslation()
{
    if ((*playerData) == NULL)
        return;
    (*playerData)->transform.translation = Vector3{
        ((float)(*playerData)->physical.x) / 100,
        ((float)(*playerData)->physical.y) / 100,
        ((float)(*playerData)->physical.z) / 100,
    };
}

void Character::_renderBox(Box box)
{
    box.render();
}
