#define is_down(b) input->buttons[b].is_down
#define pressed(b) input->buttons[b].is_down && input->buttons[b].changed
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

// things to do
// refactor duplicate code to seperate reusable functions or maybe classes?
// add score
// maybe a start delay
// choice to do player vs player or player vs cpu

//player fields
float player1_py = 0.0f;
float player1_dp = 0.0f;
float player2_px = 80.f;
float player1_px = -80.f;
float player2_py = 0.0f;
float player2_dp = 0.0f;
float speed = 50.0f;
float player_half_size_x = 2.5f;
float player_half_size_y = 12.f;
// arena fields
float arena_half_size_x = 85.f;
float arena_half_size_y = 45.f;
// ball fields
float ball_p_x = 0, ball_p_y = 0, ball_dp_x = 50, ball_dp_y = 0;//no acceleration yet
float ball_half_size = 1;

static void simulate_game(Input* input, float delta) {
	clear_screen(0x2f2235);
	// Arena stuff
	draw_rect(0,0,arena_half_size_x,arena_half_size_y, 0x8bbf9f);

	// player 1 input stuff
	float player2_ddp = 0.0f; // units per second
	if (is_down(BUTTON_UP))player2_ddp += 2000;
	if (is_down(BUTTON_DOWN))player2_ddp -= 2000;

	// player 2 input stuff
	float player1_ddp = 0.0f; // units per second
	if (is_down(BUTTON_W))player1_ddp += 2000;
	if (is_down(BUTTON_S))player1_ddp -= 2000;

	// collision detect
	// can have array of player instead
	if ((player1_py + player_half_size_y) > arena_half_size_y) {
		player1_py = arena_half_size_y - player_half_size_y;
		player1_ddp = 0;
		player1_dp *= -0.1;
		//player1_dp *= -1; if you want bounce
	}
	else if ((player1_py - player_half_size_y) < -arena_half_size_y) {
		player1_py = -arena_half_size_y + player_half_size_y;
		player1_ddp = 0;
		player1_dp *= -0.1;
		//player1_dp *= -1; if you want bounce
	}
	if ((player2_py + player_half_size_y) > arena_half_size_y) {
		player2_py = arena_half_size_y - player_half_size_y;
		player2_ddp = 0;
		player2_dp *= -0.2;
		//player2_dp *= -2; if you want bounce
	}
	else if ((player2_py - player_half_size_y) < -arena_half_size_y) {
		player2_py = -arena_half_size_y + player_half_size_y;
		player2_ddp = 0;
		player2_dp *= -0.2;
		//player2_dp *= -2; if you want bounce
	}

	// ball
	ball_p_x += ball_dp_x * delta;
	ball_p_y += ball_dp_y * delta;
	// ball to player collisions
	if ((ball_p_x + ball_half_size) > (player2_px - player_half_size_x) &&
		(ball_p_x - ball_half_size) < (player2_px + player_half_size_x)&&
		(ball_p_y + ball_half_size) > (player2_py - player_half_size_y)&&
		(ball_p_y - ball_half_size) < (player2_py + player_half_size_y)) 
	{
		ball_p_x = player2_px - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = player2_dp;
	}
	else if((ball_p_x - ball_half_size) < (player1_px + player_half_size_x)&&
		(ball_p_x + ball_half_size) > (player1_px - player_half_size_x)&&
		(ball_p_y + ball_half_size) > (player1_py - player_half_size_y) &&
		(ball_p_y - ball_half_size) < (player1_py + player_half_size_y))
	{
		ball_p_x = player1_px + player_half_size_x + ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = player1_dp;
	}

	// ball to arena collisions
	if ((ball_p_y + ball_half_size) > arena_half_size_y) {
		ball_p_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}
	else if ((ball_p_y - ball_half_size) < -arena_half_size_y) {
		ball_p_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}
	if ((ball_p_x + ball_half_size) > arena_half_size_x) {
		ball_p_y = 0;
		ball_p_x = 0;
		ball_dp_x = 50;
		ball_dp_y = 0;
	}
	else if ((ball_p_x - ball_half_size) < -arena_half_size_x) {
		ball_p_y = 0;
		ball_p_x = 0;
		ball_dp_x = 50;

	}



	player1_ddp -= player1_dp * 10.f;
	player2_ddp -= player2_dp * 10.f;
	
	player1_py = player1_py + player1_dp * delta + player1_ddp * 0.5f * delta * delta;
	player1_dp = player1_dp + player1_ddp * delta;
	player2_py = player2_py + player2_dp * delta + player2_ddp * 0.5f * delta * delta;
	player2_dp = player2_dp + player2_ddp * delta;




	// player
	draw_rect(player1_px, player1_py, player_half_size_x, player_half_size_y, 0x8b2635);
	//other
	
	draw_rect(player2_px, player2_py, player_half_size_x, player_half_size_y, 0x8b2635);

	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0x000000);
}