#include "ShapeMovement.h"
#include <algorithm>



bool ShapeMovement::TickDown(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape)
{
	if (CanTickDown(tileGrid, shape))
	{
		std::vector<GridPosition> moveAfter;
		for (auto& tile : shape)
		{
			if (std::find(shape.begin(), shape.end(), GridPosition{ tile.y, static_cast<uint8_t>(tile.x + 1) }) != shape.end())
			{
				moveAfter.push_back(tile);
			}
			else
			{
				SwapTiles(tileGrid, tile, GridPosition{ tile.y, static_cast<uint8_t>(tile.x + 1) });
			}
		}
		while (moveAfter.size() != 0)
		{
			for (auto tile = moveAfter.begin(); tile != moveAfter.end(); ++tile)
			{
				if (!(std::find(moveAfter.begin(), moveAfter.end(),
					GridPosition{ (*tile).y, static_cast<uint8_t>((*tile).x + 1) }) != moveAfter.end()))
				{
					SwapTiles(tileGrid, *tile, GridPosition{ (*tile).y, static_cast<uint8_t>((*tile).x + 1) });
					moveAfter.erase(tile);
					break;
				}
			}
		}
		for (auto& tile : shape)
		{
			tile.x++;
		}
		return true;
	}
	return false;
}

void ShapeMovement::MoveLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape)
{
	if (CanMoveLeft(tileGrid, shape))
	{
		std::vector<GridPosition> moveAfter;
		for (auto& tile : shape)
		{
			if (std::find(shape.begin(), shape.end(), GridPosition{ static_cast<uint8_t>(tile.y - 1), tile.x }) != shape.end())
			{
				moveAfter.push_back(tile);
			}
			else
			{
				SwapTiles(tileGrid, tile, GridPosition{ static_cast<uint8_t>(tile.y - 1), tile.x });
			}
		}
		while (moveAfter.size() != 0)
		{
			for (auto tile = moveAfter.begin(); tile != moveAfter.end(); ++tile)
			{
				if (!(std::find(moveAfter.begin(), moveAfter.end(), 
					GridPosition{ static_cast<uint8_t>((*tile).y - 1), (*tile).x }) != moveAfter.end()))
				{
					SwapTiles(tileGrid, *tile, GridPosition{ static_cast<uint8_t>((*tile).y - 1), (*tile).x });
					moveAfter.erase(tile);
					break;
				}
			}
		}
		for (auto& tile : shape)
		{
			tile.y--;
		}
	}
}

void ShapeMovement::MoveRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape)
{
	if (CanMoveRight(tileGrid, shape))
	{
		std::vector<GridPosition> moveAfter;
		for (auto tile = shape.rbegin(); tile != shape.rend(); ++tile)
		{
			if (std::find(shape.begin(), shape.end(), GridPosition{ static_cast<uint8_t>((*tile).y + 1), (*tile).x }) != shape.end())
			{
				moveAfter.push_back(*tile);
			}
			else
			{
				SwapTiles(tileGrid, (*tile), GridPosition{ static_cast<uint8_t>((*tile).y + 1), (*tile).x });
			}
		}
		while (moveAfter.size() != 0)
		{
			for (auto tile = moveAfter.begin(); tile != moveAfter.end(); ++tile)
			{
				if (!(std::find(moveAfter.begin(), moveAfter.end(),
					GridPosition{ static_cast<uint8_t>((*tile).y + 1), (*tile).x }) != moveAfter.end()))
				{
					SwapTiles(tileGrid, *tile, GridPosition{ static_cast<uint8_t>((*tile).y + 1), (*tile).x });
					moveAfter.erase(tile);
					break;
				}
			}
		}
		for (auto& tile : shape)
		{
			tile.y++;
		}
	}
}

void ShapeMovement::RotateLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape)
{
	if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::LeftGun)
	{
		if (shape[0].rotaion == 0)
		{
			if ((!tileGrid[shape[3].y][shape[3].x - 1].IsOn()) && 
				(!tileGrid[shape[3].y][shape[3].x - 2].IsOn()))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ shape[3].y, static_cast<uint8_t>(shape[3].x - 2) });
				shape[3].x -= 2;
				shape[3].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 1),
					static_cast<uint8_t>(shape[2].x - 1) });
				shape[2].y++;
				shape[2].x--;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y + 2), shape[1].x });
				shape[1].y += 2;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1),
					static_cast<uint8_t>(shape[0].x + 1) });
				shape[0].y++;
				shape[0].x++;
				shape[0].rotaion = 90;
			}
		}
		else if (shape[0].rotaion == 90)
		{
			if (shape[0].y != 0 && (!tileGrid[shape[2].y - 1][shape[2].x].IsOn()) &&
				(!tileGrid[shape[2].y - 2][shape[2].x].IsOn()))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 2),
					static_cast<uint8_t>(shape[3].x + 1) });
				shape[3].y -= 2;
				shape[3].x++;
				shape[3].rotaion = 180;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1), shape[2].x });
				shape[2].y--;
				shape[2].rotaion = 180;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].x--;
				shape[1].rotaion = 180;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1), shape[0].x });
				shape[0].y++;
				shape[0].rotaion = 180;

			}
		}
		else if (shape[0].rotaion == 180)
		{
			if ((!tileGrid[shape[2].y][shape[2].x + 1].IsOn()) && (!tileGrid[shape[2].y][shape[2].x - 1].IsOn()) &&
				(!tileGrid[shape[2].y + 1][shape[2].x - 1].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ shape[0].y, static_cast<uint8_t>(shape[0].x - 2) });
				shape[0].x -= 2;
				shape[0].rotaion = 270;
				SwapTiles(tileGrid, shape[1],
					GridPosition{ static_cast<uint8_t>(shape[1].y - 1), static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].y--;
				shape[1].x--;
				shape[1].rotaion = 270;
				shape[2].rotaion = 270;
				SwapTiles(tileGrid, shape[3], GridPosition{
					static_cast<uint8_t>(shape[3].y + 1), static_cast<uint8_t>(shape[3].x + 1) });
				shape[3].y++;
				shape[3].x++;
				shape[3].rotaion = 270;
			}
		}
		else
		{
			if (shape[1].y != 0 && (!tileGrid[shape[2].y - 1][shape[2].x].IsOn()) &&
				(!tileGrid[shape[3].y - 1][shape[3].x].IsOn() &&
				(!tileGrid[shape[3].y + 1][shape[3].x].IsOn())))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 1), shape[3].x });
				shape[3].y++;
				shape[3].rotaion = 0;
				SwapTiles(tileGrid, shape[2], GridPosition{ shape[2].y, static_cast<uint8_t>(shape[2].x + 1) });
				shape[2].x++;
				shape[2].rotaion = 0;
				SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y - 1),
					static_cast<uint8_t>(shape[1].x + 2) });
				shape[1].y--;
				shape[1].x += 2;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 2),
					static_cast<uint8_t>(shape[0].x + 1) });
				shape[0].y -= 2;
				shape[0].x++;
				shape[0].rotaion = 0;
			}
		}
	}
	else if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::Straight)
	{
		if (shape[0].rotaion == 0)
		{
			if (shape[0].x > 2 && (!tileGrid[shape[2].y][shape[2].x - 1].IsOn()) && (!tileGrid[shape[2].y][shape[2].x - 2].IsOn()) &&
				(!tileGrid[shape[2].y][shape[2].x - 3].IsOn()))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 2),
					static_cast<uint8_t>(shape[3].x - 3) });
				shape[3].y -= 2;
				shape[3].x -= 3;
				shape[3].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1),
					static_cast<uint8_t>(shape[2].x - 2) });
				shape[2].y--;
				shape[2].x -= 2;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].x--;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1), shape[0].x });
				shape[0].y++;
				shape[0].rotaion = 90;
			}
		}
		else
		{
			if (shape[0].y > 0 && shape[0].y < 8 && (!tileGrid[shape[0].y - 1][shape[0].x].IsOn()) &&
				(!tileGrid[shape[0].y + 1][shape[0].x].IsOn()) && (!tileGrid[shape[0].y + 2][shape[0].x].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 1), shape[0].x });
				shape[0].y--;
				shape[0].rotaion = 0;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x + 1) });
				shape[1].x++;
				shape[1].rotaion = 0;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 1),
					static_cast<uint8_t>(shape[2].x + 2) });
				shape[2].y++;
				shape[2].x += 2;
				shape[2].rotaion = 0;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 2),
					static_cast<uint8_t>(shape[3].x + 3) });
				shape[3].y += 2;
				shape[3].x += 3;
				shape[3].rotaion = 0;
			}
		}
	}
	else if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::RightSnake)
	{
		if (shape[0].rotaion == 0)
		{
			if ((!tileGrid[shape[0].y][shape[0].x - 1].IsOn() && (!tileGrid[shape[0].y][shape[0].x - 2].IsOn())))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 2),
					static_cast<uint8_t>(shape[3].x - 1) });
				shape[3].y -= 2;
				shape[3].x--;
				shape[3].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1), shape[2].x });
				shape[2].y--;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].x--;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1), shape[0].x });
				shape[0].y++;
				shape[0].rotaion = 90;
			}
		}
		else if(shape[0].y < 9 && (!tileGrid[shape[0].y - 1][shape[0].x].IsOn()) &&
			(!tileGrid[shape[1].y + 1][shape[1].x].IsOn()))
		{
			SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 1), shape[0].x });
			shape[0].y--;
			shape[0].rotaion = 0;
			SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x + 1) });
			shape[1].x++;
			shape[1].rotaion = 0;
			SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 1), shape[2].x });
			shape[2].y++;
			shape[2].rotaion = 0;
			SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 2),
				static_cast<uint8_t>(shape[3].x + 1) });
			shape[3].y += 2;
			shape[3].x++;
			shape[3].rotaion = 0;
		}
	}
	else if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::RightGun)
	{
		if (shape[0].rotaion == 0)
		{
			if ((!tileGrid[shape[3].y][shape[3].x - 1].IsOn()) && (!tileGrid[shape[1].y][shape[1].x - 2].IsOn()))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 1),
					static_cast<uint8_t>(shape[3].x - 1) });
				shape[3].y--;
				shape[3].x--;
				shape[3].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ shape[2].y, static_cast<uint8_t>(shape[2].x - 2) });
				shape[2].x -= 2;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y + 1),
					static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].y++;
				shape[1].x--;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 2), shape[0].x });
				shape[0].y += 2;
				shape[0].rotaion = 90;
			}
		}
		else if (shape[0].rotaion == 90)
		{
			if (shape[3].y > 0 && (!tileGrid[shape[0].y - 2][shape[0].x].IsOn()) &&
				(!tileGrid[shape[1].y - 1][shape[1].x].IsOn()) && (!tileGrid[shape[1].y - 2][shape[1].x].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 2), shape[0].x });
				shape[0].y -= 2;
				shape[0].rotaion = 180;
				SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y - 2), shape[1].x });
				shape[1].y -= 2;
				shape[1].rotaion = 180;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1),
					static_cast<uint8_t>(shape[2].x + 1) });
				shape[2].y--;
				shape[2].x++;
				shape[2].rotaion = 180;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 1),
					static_cast<uint8_t>(shape[3].x + 1) });
				shape[3].y++;
				shape[3].x++;
				shape[3].rotaion = 180;
			}
		}
		else if (shape[0].rotaion == 180)
		{
			if ((!tileGrid[shape[0].y + 1][shape[0].x].IsOn()) && (!tileGrid[shape[1].y][shape[1].x - 1].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1), shape[0].x });
				shape[0].y++;
				shape[0].rotaion = 270;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x + 1) });
				shape[1].x++;
				shape[1].rotaion = 270;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1), shape[2].x });
				shape[2].y--;
				shape[2].rotaion = 270;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 2),
					static_cast<uint8_t>(shape[3].x - 1) });
				shape[3].y -= 2;
				shape[3].x--;
				shape[3].rotaion = 270;
			}
		}
		else if(shape[0].y < 9 && (!tileGrid[shape[0].y + 1][shape[0].x].IsOn()) &&
			(!tileGrid[shape[2].y + 2][shape[2].x].IsOn()))
		{
			SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 2),
				static_cast<uint8_t>(shape[3].x + 1) });
			shape[3].y += 2;
			shape[3].x++;
			shape[3].rotaion = 0;
			SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 2),
				static_cast<uint8_t>(shape[2].x + 1) });
			shape[2].y += 2;
			shape[2].x++;
			shape[2].rotaion = 0;
			SwapTiles(tileGrid, shape[1], shape[0]);
			shape[1].y++;
			shape[1].rotaion = 0;
			shape[0].y--;
			shape[0].rotaion = 0;
		}
	}
	else if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::LeftSnake)
	{
		if (shape[0].rotaion == 0)
		{
			if ((!tileGrid[shape[0].y][shape[0].x + 1].IsOn()) && (!tileGrid[shape[1].y][shape[1].x - 1].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ shape[0].y, static_cast<uint8_t>(shape[0].x + 1) });
				shape[0].x++;
				shape[0].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y - 1), shape[1].x });
				shape[1].y--;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ shape[2].y, static_cast<uint8_t>(shape[2].x - 1) });
				shape[2].x--;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 1),
					static_cast<uint8_t>(shape[3].x - 2) });
				shape[3].y--;
				shape[3].x -= 2;
				shape[3].rotaion = 90;
			}
		}
		else if(shape[3].y < 9 && (!tileGrid[shape[0].y + 1][shape[0].x].IsOn()) &&
			(!tileGrid[shape[0].y + 2][shape[0].x].IsOn()))
		{
			SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 1),
				static_cast<uint8_t>(shape[3].x + 2) });
			shape[3].y++;
			shape[3].x += 2;
			shape[3].rotaion = 0;
			SwapTiles(tileGrid, shape[2], GridPosition{ shape[2].y, static_cast<uint8_t>(shape[2].x + 1) });
			shape[2].x++;
			shape[2].rotaion = 0;
			SwapTiles(tileGrid, shape[1], GridPosition{ static_cast<uint8_t>(shape[1].y + 1), shape[1].x });
			shape[1].y++;
			shape[1].rotaion = 0;
			SwapTiles(tileGrid, shape[0], GridPosition{ shape[0].y, static_cast<uint8_t>(shape[0].x - 1) });
			shape[0].x--;
			shape[0].rotaion = 0;
		}
	}
	else if (static_cast<Shapes>(tileGrid[shape[0].y][shape[0].x].GetColor()) == Shapes::Pyramid)
	{
		if (shape[0].rotaion == 0)
		{
			if ((!tileGrid[shape[0].y][shape[0].x - 1].IsOn()) && (!tileGrid[shape[2].y][shape[2].x - 1].IsOn()))
			{
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 1),
					static_cast<uint8_t>(shape[3].x - 2) });
				shape[3].y--;
				shape[3].x -= 2;
				shape[3].rotaion = 90;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1), shape[2].x });
				shape[2].y--;
				shape[2].rotaion = 90;
				SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x - 1) });
				shape[1].x--;
				shape[1].rotaion = 90;
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1), shape[0].x });
				shape[0].y++;
				shape[0].rotaion = 90;
			}
		}
		else if (shape[0].rotaion == 90)
		{
			if (shape[0].y < 9 && (!tileGrid[shape[1].y + 1][shape[1].x].IsOn()))
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y + 1),
					static_cast<uint8_t>(shape[0].x - 1) });
				shape[0].y++;
				shape[0].x--;
				shape[0].rotaion = 180;
				shape[1].rotaion = 180;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 1),
					static_cast<uint8_t>(shape[2].x + 1) });
				shape[2].y++;
				shape[2].x++;
				shape[2].rotaion = 180;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y - 1),
					static_cast<uint8_t>(shape[3].x + 1) });
				shape[3].y--;
				shape[3].x++;
				shape[3].rotaion = 180;
			}
		}
		else if (shape[0].rotaion == 180)
		{
			if (!tileGrid[shape[1].y][shape[1].x - 1].IsOn())
			{
				SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 1),
					static_cast<uint8_t>(shape[0].x - 1) });
				shape[0].y--;
				shape[0].x--;
				shape[0].rotaion = 270;
				shape[1].rotaion = 270;
				SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y + 1),
					static_cast<uint8_t>(shape[2].x - 1) });
				shape[2].y++;
				shape[2].x--;
				shape[2].rotaion = 270;
				SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 1),
					static_cast<uint8_t>(shape[3].x + 1) });
				shape[3].y++;
				shape[3].x++;
				shape[3].rotaion = 270;
			}
		}
		else if(shape[0].y > 0 && (!tileGrid[shape[3].y - 1][shape[3].x].IsOn()) &&
			(!tileGrid[shape[3].y + 1][shape[3].x].IsOn()))
		{
			SwapTiles(tileGrid, shape[0], GridPosition{ static_cast<uint8_t>(shape[0].y - 1),
				static_cast<uint8_t>(shape[0].x + 2) });
			shape[0].y--;
			shape[0].x += 2;
			shape[0].rotaion = 0;
			SwapTiles(tileGrid, shape[3], GridPosition{ static_cast<uint8_t>(shape[3].y + 1), shape[3].x });
			shape[3].y++;
			shape[3].rotaion = 0;
			SwapTiles(tileGrid, shape[1], GridPosition{ shape[1].y, static_cast<uint8_t>(shape[1].x + 1) });
			shape[1].x++;
			shape[1].rotaion = 0;
			SwapTiles(tileGrid, shape[2], GridPosition{ static_cast<uint8_t>(shape[2].y - 1), shape[2].x });
			shape[2].y--;
			shape[2].rotaion = 0;
		}
	}
}

void ShapeMovement::RotateRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition>& shape)
{
	for (uint8_t i = 0; i < 3; ++i)
	{
		RotateLeft(tileGrid, shape);
	}
}

bool ShapeMovement::CanTickDown(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape)
{
	bool canTick{ true };
	for (auto& tile : shape)
	{
		if (tile.x == 19 || (tileGrid[tile.y][tile.x + 1].IsOn() &&
			!(std::find(shape.begin(), shape.end(), GridPosition{ tile.y, static_cast<uint8_t>(tile.x + 1) }) != shape.end())))
		{
			canTick = false;
		}
	}
	return canTick;
}

bool ShapeMovement::CanMoveLeft(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape)
{
	bool canMoveLeft{ true };
	for (auto& tile : shape)
	{
		if (tile.y == 0 || (tileGrid[tile.y - 1][tile.x].IsOn() &&
			!(std::find(shape.begin(), shape.end(), GridPosition{ static_cast<uint8_t>(tile.y - 1), tile.x }) != shape.end())))
		{
			canMoveLeft = false;
		}
	}
	return canMoveLeft;
}

bool ShapeMovement::CanMoveRight(std::vector<std::vector<TytrisTile>>& tileGrid, std::vector<GridPosition> shape)
{
	bool canMoveRight{ true };
	for (auto& tile : shape)
	{
		if (tile.y == 9 || (tileGrid[tile.y + 1][tile.x].IsOn() &&
			!(std::find(shape.begin(), shape.end(), GridPosition{ static_cast<uint8_t>(tile.y + 1), tile.x }) != shape.end())))
		{
			canMoveRight = false;
		}
	}
	return canMoveRight;
}

void ShapeMovement::SwapTiles(std::vector<std::vector<TytrisTile>>& tileGrid, GridPosition first, GridPosition second)
{
	bool firstOn = tileGrid[first.y][first.x].IsOn();
	tileGrid[first.y][first.x].TurnOn(tileGrid[second.y][second.x].IsOn());
	tileGrid[second.y][second.x].TurnOn(firstOn);
	Colors firstColor = tileGrid[first.y][first.x].GetColor();
	tileGrid[first.y][first.x].SetColor(tileGrid[second.y][second.x].GetColor());
	tileGrid[second.y][second.x].SetColor(firstColor);

}