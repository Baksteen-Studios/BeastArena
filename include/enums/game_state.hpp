#ifndef FILE_GAME_STATE_HPP
#define FILE_GAME_STATE_HPP

enum class GameState {
    Unintialized,
    MainMenu,
    Highscore,
    Lobby,
    InGame,
    EndGame,
    Paused,
    LevelDebugger,
    Error
};

#endif // FILE_GAME_STATE_HPP