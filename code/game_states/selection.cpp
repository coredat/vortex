#include <game_states/selection.hpp>
#include <game_objects/player.hpp>
#include <common/game_state.hpp>
#include <core/input/controller.hpp>
#include <core/resources/texture.hpp>
#include <core/transform/transform.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_utils.hpp>
#include <core/model/model.hpp>
#include <utilities/directory.hpp>
#include <math/vec/vec3.hpp>
#include <math/quat/quat.hpp>
#include <core/entity/entity_ref.hpp>


namespace
{
  constexpr uint32_t number_of_textures = 4;
  Core::Texture textures[number_of_textures];
  
  Core::Texture no_selection_texture;
  
  constexpr uint32_t number_of_models = 4;
  Core::Model models[number_of_models];
  
  constexpr uint32_t max_number_of_players = 4;
  uint32_t current_player_selection[max_number_of_players];
  
  Core::Entity selection_screens[max_number_of_players];
  
  Core::Model plane;
  
  Core::Entity_ref signed_in_players[4];
  
  uint32_t players_signed_in = 0;
}


void
selection_init(Core::Context &ctx,
               Core::World &world,
               Core::Camera &cam)
{
  const std::string asset_path = util::get_resource_path() + "assets/";

  // No selection texture
  if(!no_selection_texture)
  {
    const std::string tex = asset_path + "textures/no_ship.png";
    no_selection_texture = Core::Texture(tex.c_str());
  }

  // Load textures
  if(!textures[0])
  {
    uint32_t load_texture = 0;
    
    const std::string tex_01 = asset_path + "textures/ship_01.png";
    textures[load_texture++] = Core::Texture(tex_01.c_str());
    
    const std::string tex_02 = asset_path + "textures/ship_02.png";
    textures[load_texture++] = Core::Texture(tex_02.c_str());
    
    const std::string tex_03 = asset_path + "textures/ship_03.png";
    textures[load_texture++] = Core::Texture(tex_03.c_str());
    
    const std::string tex_04 = asset_path + "textures/ship_04.png";
    textures[load_texture++] = Core::Texture(tex_04.c_str());
  }
  
  // Load models
  if(!models[0])
  {
    uint32_t load_model = 0;
    const std::string model_01 = asset_path + "models/ship_01.obj";
    models[load_model++] = Core::Model(model_01.c_str());
    
    const std::string model_02 = asset_path + "models/ship_02.obj";
    models[load_model++] = Core::Model(model_02.c_str());
    
    const std::string model_03 = asset_path + "models/ship_03.obj";
    models[load_model++] = Core::Model(model_03.c_str());
    
    const std::string model_04 = asset_path + "models/ship_04.obj";
    models[load_model++] = Core::Model(model_04.c_str());
  }
  
  // Set player selections
  {
    players_signed_in = 0;
    
    for(uint32_t p = 0; p < max_number_of_players; ++p)
    {
      current_player_selection[p] = p;
    }
  }
  
  // Selection Screens
  {
    const std::string plane_path = asset_path + "models/unit_plane.obj";
    
    plane = Core::Model(plane_path.c_str());
    
    for(uint32_t i = 0; i < 4; ++i)
    {
      auto &sel = selection_screens[i];
      
      sel = Core::Entity(world);
      sel.set_name("Selection screen");
      sel.set_model(plane);
      sel.set_material_id(no_selection_texture.get_id());
    }
  }
}


Game_state
selection_update(Core::Context &context,
                 Core::World &world,
                 Core::Camera &cam,
                 Game_object::World_objects &objects,
                 const float dt)
{
  constexpr uint32_t number_of_controllers = 4;
  
  const Core::Input::Controller controllers[number_of_controllers]
  {
    Core::Input::Controller(context, 0),
    Core::Input::Controller(context, 1),
    Core::Input::Controller(context, 2),
    Core::Input::Controller(context, 3),
  };
  
  /*
    If p1 hits start we start.
  */
  for(const auto &ctrl : controllers)
  {
    if(ctrl.is_button_up_on_frame(Core::Input::Button::button_4) && players_signed_in > 0)
    {
      // Reset selection screen
      {
        for(auto &sel : selection_screens)
        {
          // Currently the best way to hide an entity is just
          // to create a new one :D
          sel.destroy();
        }
      }
    
      return Game_state::game_mode;
    }
  }
  
  /*
    Update sel screens
  */
    for(uint32_t i = 0; i < 4; ++i)
    {
      auto &sel = selection_screens[i];
      
      math::mat4 proj   = Core::Camera_utils::camera_get_projection_matrix(cam);
      math::mat4 view   = Core::Camera_utils::camera_get_view_matrix(cam);
      math::mat4 inv_vp = math::mat4_get_inverse(math::mat4_multiply(proj, view));
      
      const float offset = -3.f + (i * 2.f);
      
      math::vec4 screen_pos = math::vec4_init(offset, 0.f, -1.f, 1.f);
      math::vec4 world_pos  = math::mat4_multiply(screen_pos, inv_vp);
      
      math::vec3 world_pos3 = math::vec3_init(math::vec4_get_x(world_pos), math::vec4_get_y(world_pos), math::vec4_get_z(world_pos));
      math::vec3 dir        = math::vec3_normalize(world_pos3);
      
      auto intersect_plane = [](const math::vec3 plane_normal,
                                const math::vec3 plane_position,
                                const math::vec3 ray_start,
                                const math::vec3 ray_dir,
                                float &time) -> bool
      {
        // assuming vectors are all normalized
        float denom = math::vec3_dot(plane_normal, ray_dir);
        if (denom > 1e-6)
        {
          math::vec3 p0l0 = math::vec3_subtract(plane_position, ray_start);
          time = math::vec3_dot(p0l0, plane_normal) / denom;
          return (time >= 0);
        } 
     
        return false; 
      };
      
      const Core::Transform cam_trans = cam.get_attached_entity().get_transform();
      
      const math::vec3 fwd = math::vec3_scale(cam_trans.get_forward(), 0.2);
      const math::vec3 plane_pos = math::vec3_add(cam_trans.get_position(), fwd);
      
      float time;
      const bool did_intersect = intersect_plane(math::vec3_init(0, 0, -1),
                                                 plane_pos,
                                                 cam_trans.get_position(),
                                                 dir,
                                                 time);
      
      if(did_intersect)
      {
        math::vec3 final_scale = math::vec3_scale(dir, time);
        math::vec3 final_pos = math::vec3_add(cam_trans.get_position(), final_scale);

        // Place card
        Core::Transform trans;
        trans.set_position(math::vec3_init(math::vec3_get_x(final_pos), math::vec3_get_y(final_pos), math::vec3_get_z(final_pos)));
        constexpr float scale = 0.05f;
        trans.set_scale(math::vec3_init(scale, 1, scale * math::g_ratio()));
        trans.set_rotation(math::quat_init_with_axis_angle(1, 0, 0, -math::quart_tau()));
        
        sel.set_transform(trans);
      }
    }
  
  /*
    Add players as the push their buttons.
  */
  for(uint32_t i = 0; i < number_of_controllers; ++i)
  {
  
    if(!signed_in_players[i].is_valid())
    {
      if(controllers[i].is_button_down_on_frame(Core::Input::Button::button_0))
      {
        auto new_player = new Game_object::Player(world, context, i);
        signed_in_players[i] = new_player->get_entity();
        
        objects.push_object(new_player);
        
        ++players_signed_in;
      }
    }
    
    if(controllers[i].is_button_down_on_frame(Core::Input::Button::button_0))
    {
      current_player_selection[i] = (current_player_selection[i] + 1) % number_of_textures;
      const uint32_t selection = current_player_selection[i];
      
      signed_in_players[i].set_model(models[selection]);
      signed_in_players[i].set_material_id(textures[selection].get_id());

      // Add selection screen.
      selection_screens[i].set_model(plane);
      selection_screens[i].set_material_id(textures[selection].get_id());
    }
  }
  
  return Game_state::selection;
}