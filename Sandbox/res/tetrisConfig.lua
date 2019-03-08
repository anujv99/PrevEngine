Snake_Config = {
	UpdateInterval = 0.04, -- In Seconds
	InitialLength = 10,
	StartPos = {
		X = 10,
		Y = 10
	},
	TileSize = 16,
	LengthToIncrease = 1
}

Tetris_Config = {
	UpdateInterval = 0.2, -- In Seconds
	TileDivision = {
		X = 10,
		Y = 16
	},
	TileSize = {
		X = 32,
		Y = 32
	}
}

Window = {
	Title = "Tetris",
	Width = Tetris_Config.TileDivision.X * Tetris_Config.TileSize.X,
	Height = Tetris_Config.TileDivision.Y * Tetris_Config.TileSize.Y,
	IsVsync = false
}

Log = {
	IsLogEnabled = true,
	ShowFPS = false -- Will not work if log is disabled
}