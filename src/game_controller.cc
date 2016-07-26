#include <game_controller.h>

namespace openrayman
{
	game_controller::game_controller(engine &active_engine, game &active_game, renderer &active_renderer) :
		m_engine(active_engine), m_game(active_game), m_renderer(active_renderer)
	{

	}

	game_controller::~game_controller()
	{

	}

	void game_controller::tick(double delta, std::uint64_t update, input_state &input, input_state &last_input)
	{

	}

	void game_controller::timed_tick(std::uint64_t update, input_state &input, input_state &last_input)
	{

	}

	void game_controller::render()
	{

	}
}