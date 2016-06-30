#include <game_objects/player.hpp>
#include <core/world/world.hpp>


namespace Game_object {


Player::Player(Core::World &world)
: Game_object(world)
{
}


void
Player::on_start()
{
}


void
Player::on_message(const uint32_t id, void *data)
{
}



} // ns