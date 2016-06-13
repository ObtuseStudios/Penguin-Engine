/*
#include <Engine\Engine.h>
#include <vector>

using namespace Penguin;

const int TILE = 64;
const float GRAVITY = 0.5;
const float FRICTION = 0.6f;
const int PLAYER_MAX_VEL = 5;
const int PLAYER_JUMP = 15;
const int FRAME_RATE = 1000 / 70;
const Vector SCREEN_SIZE = Vector(1280, 704);
const Colour SCREEN_COLOUR = Colour(0, 155, 130);

const std::string PATH = "Assets/Player->png";
const std::string WINDOW_TITLE = "Cameron's Game";

int main(int argc, char** argv)
{
	Engine Engine;
	
	Engine.Window.Create(WINDOW_TITLE, SCREEN_SIZE, SCREEN_COLOUR);

	std::vector<Rectangle> level;
	std::vector<std::string> levelFile = Engine.FileReader.loadTextFile("level.txt");

	for (int i = 0; i < levelFile.size(); i++)
	{
		for (int j = 0; j < levelFile[i].size(); j++)
		{
			if (levelFile[i][j] == '*')
			{
				Rectangle temp(Vector(j * TILE, i * TILE), Vector(TILE, TILE));
				level.push_back(temp);
			}
		}

		Engine.Debug.Print(levelFile[i]);
	}

	Rectangle player(Vector(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2), Vector(64 , 64), Colour(255));

	float playerX = 0;
	float playerY = 0;

	int frameNumber = 0;
	int collisionDirection = 0;
	bool playerGrounded = false;

	Engine.Window.Camera.Init(Engine.Window.GetWindowSize(), Vector(1000, 1000));

	while (Engine.Window.GetWindowState())
	{
		Engine.FPS.Start();
		Engine.Window.Clear();
		Engine.Input.Check();

		Engine.Window.Camera.Follow(Player->Position, Player->Scale);

		playerY += GRAVITY;
		
		playerGrounded = true;

		for (int i = 0; i < level.size(); i++)
		{
			
			collisionDirection = Engine.Physics.checkCollision(player, level[i], false);

			if (collisionDirection == Engine.Physics.bottom) 
			{ 
				Player->Position.y = level[i].Position.y - level[i].Scale.y;
				playerY = 0;
			}
			else playerGrounded = false;
			
			if (collisionDirection == Engine.Physics.top) 
			{ 
				Player->Position.y = level[i].Position.y + level[i].Scale.y;
				playerY = 0; 
			}

			if (collisionDirection == Engine.Physics.left) 
			{ 
				Player->Position.x = level[i].Position.x + level[i].Scale.x; 
				playerX = 0; 
			};

			if (collisionDirection == Engine.Physics.right)
			{ 
				Player->Position.x = level[i].Position.x - level[i].Scale.x; 
				playerX = 0; 
			};
			

			if (Player->Position.y + playerY >= 720 - 64- Player->Scale.y)
			{
				Player->Position.y = 720 - 64 - Player->Scale.y;
				playerY = 0;
			}
		}

		if (Engine.Input.ButtonHeld(Engine.Key.LEFT)) { playerX -= PLAYER_MAX_VEL; Player->Flip = Vector(0, 0); }
		if (Engine.Input.ButtonHeld(Engine.Key.RIGHT)) { playerX += PLAYER_MAX_VEL; Player->Flip = Vector(1, 0); }
		if (Engine.Input.ButtonReleased(Engine.Key.UP) && playerGrounded) { playerY -= PLAYER_JUMP; }

		playerX *= FRICTION;

		Player->Position.x += playerX;
		Player->Position.y += playerY;

		Player->Draw(&Engine.Window);

		for (int i = 0; i < level.size(); i++) level[i].Draw(&Engine.Window);

		Engine.Window.Draw();

		if (frameNumber >= FRAME_RATE)
		{
			Engine.Debug.Print(Engine.FPS.Update(FRAME_RATE));
			frameNumber = 0;
		}
		else Engine.FPS.Update(FRAME_RATE);
		
		frameNumber++;
	}


	Engine.Debug.Quit();
	return 0;
}
*/

/*
#include <Engine\Engine.h>
int main(int argc, char** argv)
{
	using namespace Penguin;

	Penguin::Engine Engine;
	Engine.Window.Create("Ball", Vector(1280, 720));

	Engine.Sound.AddSong("Assets/Song.wav");
	Engine.Sound.AddSong("Assets/Third.wav");
	Engine.Sound.AddSong("Assets/Second.wav");

	int bounceSound = Engine.Sound.LoadSoundEffect("Assets/Bounce.wav");

	Text title;
	Text instructions;
	title.Load(Engine.Window.GetRenderer(), "Assets/Cornerstone.ttf", "0", 72);
	instructions.Load(Engine.Window.GetRenderer(), "Assets/Cornerstone.ttf", "Arrows to apply force", 50);
	instructions.Position = Vector(1280 / 2 - instructions.Scale.x / 2, 720 / 2 - instructions.Scale.y * 2);

	Sprite Player(Vector(1280 / 2 - 25, 720 / 2 - 25), Vector(50, 50), Colour(255));
	Player->center = Vector(25, 25);
	Player->Load(Engine.Window.GetRenderer(), "Assets/Player->png");
	Player->rotation = 45;
	
	float friction = 0.95f;
	float gravity = 0.5f;
	float airResistance = 0.995f;
	Vector velocity = Vector(0, 0);

	Vector speed = Vector(0, 0);

	while (Engine.Window.GetWindowState())
	{
		Engine.Sound.PlayAlbum();

		Engine.Window.Clear();
		Engine.Input.Check();

		if (Engine.Input.ButtonHeld(Engine.Key.UP))	speed.y--;
		if (Engine.Input.ButtonHeld(Engine.Key.DOWN)) speed.y++;
		if (Engine.Input.ButtonHeld(Engine.Key.LEFT)) speed.x--;
		if (Engine.Input.ButtonHeld(Engine.Key.RIGHT)) speed.x++;

		if (Engine.Input.ButtonReleased(Engine.Key.UP) || Engine.Input.ButtonReleased(Engine.Key.DOWN))
		{
			velocity.y += speed.y;
			speed.y = 0;
		}

		if (Engine.Input.ButtonReleased(Engine.Key.LEFT) || Engine.Input.ButtonReleased(Engine.Key.RIGHT))
		{
			velocity.x += speed.x;
			speed.x = 0;
		}

		velocity.y += gravity;
		Player->rotation += velocity.x;

		//Check if ball is on floor
		if (Player->Position.y + velocity.y >= 720 - Player->Scale.y)
		{
			if (velocity.y > 0.1)
			{
				velocity.y *= friction;
				velocity.x *= friction;

				//Reject floor
				velocity.y += (720 - Player->Scale.y) - Player->Position.y;
			}
			else velocity.y = 0;

			if (velocity.y > 2)	Engine.Sound.PlaySFX(bounceSound, int(velocity.y) * 10);
		}

		//Check if not at the top of screen
		if (Player->Position.y <= 0) 
		{ 
			//Friction
			velocity.y *= friction; 
			velocity.x *= friction; 

			//Reject the floor
			velocity.y += 0 - Player->Position.y;
			if (velocity.y > 2)	Engine.Sound.PlaySFX(bounceSound, int(velocity.y) * 10);
		}

		if (Player->Position.x + velocity.x >= 1280 - Player->Scale.x) 
		{ 
			//Friction
			velocity.x *= friction; 
			velocity.y *= friction; 

			//Rejct the wall
			velocity.x += (1280 - Player->Scale.x) - Player->Position.x;
			if (velocity.x > 2)	 Engine.Sound.PlaySFX(bounceSound, int(velocity.x) * 10);
		}

		if (Player->Position.x + velocity.x <= 0) 
		{
			//Friction
			velocity.x *= friction;
			velocity.y *= friction;

			//Bounce
			velocity.x += 0 - Player->Position.x;

			if (velocity.x > 2)	Engine.Sound.PlaySFX(bounceSound, int(velocity.x) * 10);
		}
		
		title.SetText(Engine.Window.GetRenderer(), 
			"Y: " + to_string(int(speed.y)) +
			"    X: " + to_string(int(speed.x)));

		title.Position = Vector(1280 / 2 - title.Scale.x / 2, 720 / 2 - title.Scale.y / 2);

		velocity.y *= airResistance;
		velocity.x *= airResistance;

		Player->Position.x += velocity.x;
		Player->Position.y += velocity.y;

		instructions.Draw(Engine.Window.GetRenderer());
		title.Draw(Engine.Window.GetRenderer());
		Player->Draw(Engine.Window.GetRenderer());

		Engine.Window.Draw();
	}

	return 0;
}
*/

/*
#include <Engine\Engine.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int main(int argc, char** argv)
{
	using namespace Penguin;

	Engine Engine;

	Engine.Window.Create("Mouser", Vector(WIN_WIDTH, WIN_HEIGHT));

	Rectangle Box(Vector(WIN_WIDTH / 2, WIN_HEIGHT / 2), Vector(50, 50));
	Engine.Window.Camera.Init(Engine.Window.GetWindowSize());

	Rectangle random(Vector(100, 100));

	while (Engine.Window.GetWindowState())
	{
		Engine.Window.Clear();
		Engine.Input.Check();

		Engine.Window.Camera.Follow(Box.Position, Box.Scale, 0.005);

		Box.Position = Vector(
			Engine.Input.GetMousePos(Engine.Window.Camera.position).x - Box.Scale.x / 2,
			Engine.Input.GetMousePos(Engine.Window.Camera.position).y - Box.Scale.y / 2);

		Box.Draw(&Engine.Window);
		random.Draw(&Engine.Window);

		Engine.Window.Draw();
	}

	Engine.Debug.Quit();
	return 0;
}
*/
/*
//Librarys
#include <Engine\Engine.h>
#include <vector>

//use engine namespace
using namespace Penguin;

//Define values
const int TILE = 32;
const int ROWS = 10;
const int COLUMNS = 18;
const int TIMESTEP = 1000 / 20;

const int SHORT_TIME_BETWEEN = 1;
const int NORMAL_TIME_BETWEEN = 10;

const int SCREEN_WIDTH = ROWS * TILE;
const int SCREEN_HEIGHT = COLUMNS * TILE;

Rectangle Respawn();

int main(int argc, char** argv)
{
	
	//Setup window
	Engine Engine;
	Engine.Window.Create("Tetris", Vector(SCREEN_WIDTH, SCREEN_HEIGHT));

	//Stores all static blocks
	std::vector<Rectangle> Board;

	//The currently moving block
	Rectangle activeBlock = Respawn();

	//Move when this passes a value
	int Timer = 0;

	//Current time between
	int currentTimeBetween = 0;

	//Loop
	while (Engine.Window.GetWindowState())
	{
		//All engine required functions
		Engine.FPS.Start();
		Engine.Window.Clear();
		Engine.Input.Check();

		//Allowed to move left anf right
		bool allowedMoveRight = true;
		bool allowedMoveLeft = true;

		//Only move down every so frames
		Timer++;

		//Only move down ever ten frames
		if (Timer >= currentTimeBetween)
		{
			//Move down
			activeBlock.Position.y += TILE;
			Timer = 0;
		}

		//Check collision
		for (int i = 0; i < Board.size(); i++)
		{
			//Bellow
			if (activeBlock.Position.y > Board[i].Position.y - Board[i].Scale.y)
			{
				if (activeBlock.Position.x < Board[i].Position.x + Board[i].Scale.x)
				{
					if (activeBlock.Position.x > Board[i].Position.x - Board[i].Scale.x)
					{
						activeBlock.Position.y -= activeBlock.Position.y - (Board[i].Position.y - Board[i].Scale.y);
						Board.push_back(activeBlock);
						activeBlock = Respawn();
					}
				}
			}
		}

		//Get input and move appropriatly
		if (Engine.Input.ButtonHeld(Engine.Key.LEFT) && allowedMoveLeft) activeBlock.Position.x -= TILE;
		if (Engine.Input.ButtonHeld(Engine.Key.RIGHT) && allowedMoveRight) activeBlock.Position.x += TILE;

		if (Engine.Input.ButtonHeld(Engine.Key.DOWN)) currentTimeBetween = SHORT_TIME_BETWEEN;
		else currentTimeBetween = NORMAL_TIME_BETWEEN;

		//Check Collision on floor
		if (activeBlock.Position.x < 0) activeBlock.Position.x = 0;
		if (activeBlock.Position.x + activeBlock.Scale.x > SCREEN_WIDTH) activeBlock.Position.x = SCREEN_WIDTH - activeBlock.Scale.x;

		if (activeBlock.Position.y + activeBlock.Scale.y > SCREEN_HEIGHT)
		{
			activeBlock.Position.y = SCREEN_HEIGHT - activeBlock.Scale.y;
			Board.push_back(activeBlock);
			activeBlock = Respawn();
		}

		//Draw all the blocks
		activeBlock.Draw(&Engine.Window);
		for (int i = 0; i < Board.size(); i++) Board[i].Draw(&Engine.Window);

		//Draw and set FPS
		Engine.Window.Draw();
		Engine.FPS.Update(TIMESTEP);
	}

	//Close
	Engine.Debug.Quit();
	return 0;
}

Rectangle Respawn()
{
	//Return rectangle at center and top
	return Rectangle(Vector(SCREEN_WIDTH / 2, 0), Vector(TILE, TILE));
}
*/

/*
#include <Engine\Engine.h>

using namespace Penguin;

int main(int argc, char** argv)
{
	Engine Engine;

	Engine.Window.Create();

	TextInput txtIn(Engine.Window.GetRenderer(), "Assets/Cornerstone.ttf", 16, "BLAH BLAH", Colour(255, 0, 0), 250);
	txtIn.SetPosition(Vector(100, 100));

	while (Engine.Window.GetWindowState())
	{
		Engine.Window.Clear();
		Engine.Input.Check();

		txtIn.Update(Engine.Window);

		Engine.Window.Draw();
	}

	Engine.Debug.Quit();
	return 0;
}
*/

/*
#include <Engine\Engine.h>

#define DEV_MODE

using namespace Penguin;

enum state
{
	Searching, Found
};

int main(int argc, char** argv)
{
	state enemyState = state::Searching;

	const int PLAYER_ID = 4;
	const int ENEMEY_ID = 0;

	Engine Engine;

	Engine.Window.Create("Demo", Vector(1280, 720));

	Rectangle LevelBounds(Vector(0, 0), Vector(1280, 720));
	Sprite MapImage;
	
	MapImage.Load(Engine.Window.GetRenderer(), "Assets/Tiles.png", true);

	Engine.Window.Camera.Init(Engine.Window.GetWindowSize(), 1, LevelBounds.Scale);
	
	TileMap tileMap("Assets/Map.csv", &MapImage, Vector(3, 4), Vector(32, 18), Vector(40, 40));
	std::map<int, Tile>* level = tileMap.GetEntireLevel();
	Tile* Player = tileMap.GetFirstByID(PLAYER_ID);
	Tile* Enemy = tileMap.GetFirstByID(ENEMEY_ID);

	//level->erase(Enemy->positionInLevel);
	
	Player->Collider.ObjType = Type::Rigid;

	//ParticleSystem particleSys(100, "Assets/Dot.png", Engine.Window.GetRenderer(), DestroyTypes::Colourize);

	Engine.Sound.AddSong("Assets/Song.wav");
	Engine.Sound.AddSong("Assets/Second.wav");
	Engine.Sound.AddSong("Assets/Third.wav");

#ifdef DEV_MODE
	string currentFPS = "";
	bool showStats = true;

	Text readout(Vector(30, 30));
	Sprite textBackground(Vector(20, 20));
	textBackground.Load(Engine.Window.GetRenderer(), "Assets/Brick.png");
	textBackground.Col = Colour(0, 0, 0, 200);
	readout.Load(Engine.Window.GetRenderer(), "Assets/Cornerstone.ttf", "Demo", 24);
	readout.Col = Colour(255, 255, 255);
#endif


	Engine.PathFinding.ClearPathToGoal();

	Engine.PathFinding.foundGoal = false;
	Engine.PathFinding.initializedStartGoal = false;

	while (Engine.Window.GetWindowState())
	{
		Engine.FPS.Start();
		Engine.Window.Clear();
		Engine.Input.Check(&Engine.Window);

		//Engine.Sound.PlayAlbum();

		if (Engine.Input.ButtonHeld(Engine.Key.RIGHT)) Player->Velocity.x += 250 * Engine.FPS.deltaTime;
		if (Engine.Input.ButtonHeld(Engine.Key.LEFT)) Player->Velocity.x -= 250 * Engine.FPS.deltaTime;
		if (Engine.Input.ButtonHeld(Engine.Key.DOWN)) Player->Velocity.y += 250 * Engine.FPS.deltaTime;
		if (Engine.Input.ButtonHeld(Engine.Key.UP)) Player->Velocity.y -= 250 * Engine.FPS.deltaTime;

		if (Engine.PathFinding.changedCell(Player->Position))
		{
			Engine.PathFinding.ClearPathToGoal();

			Engine.PathFinding.initializedStartGoal = false;
			Engine.PathFinding.foundGoal = false;

			Engine.Debug.Print("Recalculating");

			enemyState = state::Searching;
		}

		if (enemyState == state::Found)
		{
			Enemy->Position = Engine.PathFinding.NextPathPos(Player);
		}

		while (enemyState == state::Searching)
		{
			Engine.Window.Clear();

			Vector normPos = Player->Position;

			normPos.x /= 40;
			normPos.y /= 40;
			
			normPos.x = int(normPos.x);
			normPos.y = int(normPos.y);

			normPos.x *= 40;
			normPos.y *= 40;

			Engine.PathFinding.FindPath(Engine.Window.GetRenderer(), Enemy->Position, normPos, level);

			if (Engine.PathFinding.foundGoal)
				enemyState = state::Found;

			tileMap.Draw(&Engine.Window);

			LevelBounds.DrawOutline(&Engine.Window);
			Engine.Window.Draw();

		}


		Player->MoveX();
		for (int i = 0; i < level->size(); i++)
			if ((*level)[i].positionInLevel != Player->positionInLevel && (*level)[i].positionInLevel != Enemy->positionInLevel)
				Engine.Physics.ResolveHorizontalCollision(Player, &(*level)[i]);

		Player->MoveY();
		for (int i = 0; i < level->size(); i++)
			if ((*level)[i].positionInLevel != Player->positionInLevel && (*level)[i].positionInLevel != Enemy->positionInLevel)
				Engine.Physics.ResolveVerticalCollision(Player, &(*level)[i]);

#ifdef DEV_MODE
		if (Engine.Input.ButtonReleased(Engine.Key.L)) { showStats = !showStats; Engine.Debug.Print("Dev mode!"); }
		
		if (showStats)
		{
			if (Engine.Input.ButtonHeld(Engine.Key.P)) Engine.Window.Camera.scale -= Engine.FPS.deltaTime;
			if (Engine.Input.ButtonHeld(Engine.Key.O)) Engine.Window.Camera.scale += Engine.FPS.deltaTime;
		}
#endif

		Engine.Window.Camera.Follow(Vector(Player->Position.x, Player->Position.y), 3 * Engine.FPS.deltaTime);

		Engine.FPS.EndMovement();

		//particleSys.RenderParticle(&Engine.Window, Player->Position, Player->Scale, 50);
		tileMap.Draw(&Engine.Window);

		LevelBounds.DrawOutline(&Engine.Window);

#ifdef DEV_MODE
		if (showStats)
		{
			readout.SetText(Engine.Window.GetRenderer(),
				"Screen: " + to_string(int(Engine.Window.GetWindowSize().x)) + " x " + to_string(int(Engine.Window.GetWindowSize().y)) + "\n" +
				currentFPS + "\n" +
				"Player position: " + to_string(Player->Position.x) + " x " + to_string(Player->Position.y) + "\n" +
				"Enemy position: " + to_string(Enemy->Position.x) + " x " + to_string(Enemy->Position.y) + "\n" +
				"Camera Scale: " + to_string(Engine.Window.Camera.scale) + " Position: " +
				to_string(Engine.Window.Camera.position.x) + " x " + to_string(Engine.Window.Camera.position.x) + "\n" +
				"Enemy state: " + to_string(enemyState),
				1000);

			textBackground.Scale = readout.Scale;
			textBackground.Scale.x *= 0.8f;
			textBackground.Scale.y *= 1.2f;

			textBackground.Draw(&Engine.Window, true);
			readout.Draw(&Engine.Window, true);
		}
#endif

		//Engine.Window.Camera.adjustMouse(&Engine.Input);
		Engine.Window.Draw();

#ifdef DEV_MODE
		currentFPS = Engine.FPS.Update(1000 / 70);
#else
		Engine.FPS.Update(1000 / 70);
#endif

		//Engine.Debug.Wait(300);
	}

	Engine.Debug.Quit();
	return 0;
}
*/

//Use engine header
#include <Engine\Engine.h>

//Namespace
using namespace Penguin;

//Developer mode variable
#define DEV_MODE

//Tilemap data
#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 22
#define TILE_IMAGE_ROWS 2
#define TILE_IMAGE_COLUMNS 2
#define LEVEL_FILE "Assets/Level1.csv"
#define TILE_IMAGE "Assets/Tiles.png"

//Sprites
#define FONT_FILE "Assets/Cornerstone.ttf"

//Tilemap IDs
#define PLAYER_ID 1

//Constant values
#define TILE 12
#define GAME_SPEED 120
#define CAMERA_SCALE 3
#define CAMERA_SPEED 5
#define GAME_TITLE "Platformer"
#define SCREEN_WIDTH LEVEL_WIDTH * TILE * CAMERA_SCALE
#define SCREEN_HEIGHT LEVEL_HEIGHT * TILE * CAMERA_SCALE

//Player properties
#define JUMP_POWER 6
#define MOVE_SPEED 40
#define PLAYER_SIZE_SMALL TILE
#define PLAYER_SIZE_LARGE TILE * 2

//Physics constants
#define FRICTION 0.8
#define MAX_VELOCITY 250
#define GRAVITY_CONSTANT 15

//Player variables
bool pGrounded = false;		//Grounded or not
bool pJumping = true;		//Player jumping or not
float pSpeed = MOVE_SPEED;	//Movement speed

//Entry
int main(int argc, char** argv)
{
	//Create engine object
	Engine Engine;

	//Create window
	Engine.Window.Create(GAME_TITLE, Vector(SCREEN_WIDTH, SCREEN_HEIGHT));

	//Load the tilemapp image
	Sprite TileSource;
	TileSource.Load(Engine.Window.GetRenderer(), TILE_IMAGE, true);

	//Load the tilemap
	TileMap Tiles(LEVEL_FILE, &TileSource, Vector(TILE_IMAGE_ROWS, TILE_IMAGE_COLUMNS), Vector(LEVEL_WIDTH, LEVEL_HEIGHT), Vector(TILE, TILE));
	
	//Get all the objects in the tilemap
	map<int, Tile>* Walls = Tiles.GetEntireLevel();
	Tile* Player = Tiles.GetFirstByID(PLAYER_ID);

	//Setup the in game camera to zoom in by 2
	Engine.Window.Camera.Init(Vector(SCREEN_WIDTH, SCREEN_HEIGHT), 
		CAMERA_SCALE, Vector(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3), 
		Vector(0, SCREEN_HEIGHT));

	//Move right keys
	const unsigned static int KEY_RIGHT = Engine.Key.RIGHT;
	const unsigned static int ALT_KEY_RIGHT = Engine.Key.D;

	//Move left keys
	const unsigned static int KEY_LEFT = Engine.Key.LEFT;
	const unsigned static int ALT_KEY_LEFT = Engine.Key.A;

	//Sprint the player
	const unsigned static int KEY_SPRINT = Engine.Key.SHIFT;
	const unsigned static int ALT_KEY_SPRINT = Engine.Key.X;
	
	//Buttons to jump
	const unsigned static int KEY_JUMP = Engine.Key.SPACE;
	const unsigned static int NEG_KEY_JUMP = Engine.Key.W;
	const unsigned static int INV_KEY_JUMP = Engine.Key.Z;
	const unsigned static int ALT_KEY_JUMP = Engine.Key.UP;

	//Dev mode variables
#ifdef DEV_MODE
	Text DevModeMessage(Vector(50, 400));
	DevModeMessage.Load(Engine.Window.GetRenderer(), FONT_FILE, "Null", 24);
#endif

	string currentFPS = "Undefined";

	//Enter main loop
	while (Engine.Window.GetWindowState())
	{
		//Engine starting
		Engine.FPS.Start();
		Engine.Window.Clear();
		Engine.Input.Check(&Engine.Window);

		//Check for jump button and set grounded
		if (pGrounded) 
		{
			if (Engine.Input.ButtonDown(KEY_JUMP) ||
				Engine.Input.ButtonDown(ALT_KEY_JUMP) ||
				Engine.Input.ButtonDown(INV_KEY_JUMP) ||
				Engine.Input.ButtonDown(NEG_KEY_JUMP))
				{
					Player->Velocity.y = -JUMP_POWER; 
					pGrounded = false; 
				}
		}

		//Set jumping variable
		if (Engine.Input.ButtonHeld(KEY_JUMP) || Engine.Input.ButtonHeld(ALT_KEY_JUMP)
			|| Engine.Input.ButtonHeld(INV_KEY_JUMP) || Engine.Input.ButtonHeld(NEG_KEY_JUMP))
				pJumping = true;

		else pJumping = false;

		//Check if jump is held, apply larger jump
		if (Player->Velocity.y < 0 && !pJumping) Player->Velocity.y = max(int(Player->Velocity.y), -JUMP_POWER * 16);

		//Check if moving left
		if (Engine.Input.ButtonHeld(KEY_LEFT) || Engine.Input.ButtonHeld(ALT_KEY_LEFT))
		{
			Player->Velocity -= pSpeed * Engine.FPS.deltaTime;
			Player->Flip = Vector(1, 0);
		}

		//Check if moving to the right
		if (Engine.Input.ButtonHeld(KEY_RIGHT) || Engine.Input.ButtonHeld(ALT_KEY_RIGHT))
		{
			Player->Velocity += pSpeed * Engine.FPS.deltaTime; 
			Player->Flip = Vector(0, 0);
		}

		//For finding delta time
		Engine.FPS.EndMovement();

		//Apply gravity and friction
		Player->Velocity.x *= FRICTION;
		Player->Velocity.y += GRAVITY_CONSTANT * Engine.FPS.deltaTime;

		//Check velocity is not too high
		if (Player->Velocity.x > MAX_VELOCITY) Player->Velocity.x = MAX_VELOCITY;
		if (Player->Velocity.x < -MAX_VELOCITY) Player->Velocity.x = -MAX_VELOCITY;
		if (Player->Velocity.y > MAX_VELOCITY) Player->Velocity.y = MAX_VELOCITY;
		if (Player->Velocity.y < -MAX_VELOCITY) Player->Velocity.y = -MAX_VELOCITY;

		//Apply velocity to postiion
		Player->Position.x += Player->Velocity.x;
		Player->Position.y += Player->Velocity.y;

		//Loop through all walls
		typedef std::map<int, Tile>::iterator it_type;
		for (it_type iterator = Walls->begin(); iterator != Walls->end(); iterator++)
		{
			//Check thats its not the player
			if (iterator->second.ID != PLAYER_ID)
			{
				//Get collision direction
				Manifold collisionDir = Engine.Physics.CheckCollision(*Player, iterator->second);

				//Do basic collision resolve
				Engine.Physics.ResolveCollision(&collisionDir, *Player, CAMERA_SCALE);

				//Ignore
				if (collisionDir.Dir == Direction::None) continue;

				//For collision bellow the player
				if (collisionDir.Dir == Direction::Down)
				{
					pGrounded = true;
				}
			}

			//Occlusion culling
			if (iterator->second.Position.x > Engine.Window.Camera.position.x + SCREEN_WIDTH / CAMERA_SCALE)
				iterator->second.shouldDraw = false;
			
			else if (iterator->second.Position.x + TILE < Engine.Window.Camera.position.x / CAMERA_SCALE)
				iterator->second.shouldDraw = false;

			else iterator->second.shouldDraw = true;
		}

		//Camera follows player
		Engine.Window.Camera.Follow(Player->Position, Engine.FPS.deltaTime * CAMERA_SPEED);

		//Drawing
		Tiles.Draw(&Engine.Window);

		//Dev mode messages
#ifdef DEV_MODE
		string Message = "Player position: " + to_string(int(Player->Position.x)) + " x " + to_string(int(Player->Position.y)) + "\n" +
			"Player scale: " + to_string(int(Player->Scale.x)) + " x " + to_string(int(Player->Scale.y)) + "\n" +
			"Player grounded: " + to_string(pGrounded) + "\n" +
			"Player move speed: " + to_string(int(pSpeed)) + "\n" +
			currentFPS + "\n";

		DevModeMessage.SetText(Engine.Window.GetRenderer(), Message, SCREEN_WIDTH);
		DevModeMessage.Draw(&Engine.Window, true);
#endif

		//Closing up
		Engine.Window.Draw();
		currentFPS = Engine.FPS.Update(1000 / GAME_SPEED);
	}

	//Close the program
	Engine.Debug.Quit();

	return 0;
}