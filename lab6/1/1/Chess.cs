using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;

namespace ChessLab
{
    public class Chess
    {
        private const float QUAD_SIZE = 40f;
        private const float BOARD_SCALE = 90f;
        private const float PIECE_HEIGHT_OFFSET = 20f;
        private const float ROTATION_ANGLE = -90f;

        private readonly Color4 BLACK_COLOR = new Color4(0.2f, 0.2f, 0.2f, 1f);
        private readonly Color4 WHITE_COLOR = Color4.White;

        private readonly Model rook;
        private readonly Model knight;
        private readonly Model bishop;
        private readonly Model queen;
        private readonly Model king;
        private readonly Model pawn;
        private readonly Model board;

        public Chess()
        {
            rook = LoadModel("models/Rook.obj");
            knight = LoadModel("models/Knight.obj");
            bishop = LoadModel("models/Bishop.obj");
            queen = LoadModel("models/Queen.obj");
            king = LoadModel("models/King.obj");
            pawn = LoadModel("models/Pawn.obj");
            board = LoadModel("models/ChessBoard.obj");
        }

        private Model LoadModel(string filePath)
        {
            var model = new Model();
            model.LoadModel(filePath);
            return model;
        }

        public void Draw()
        {
            DrawBoard();

            GL.PushMatrix();
            GL.Translate(0f, PIECE_HEIGHT_OFFSET, 0f);
            GL.Rotate(ROTATION_ANGLE, 1f, 0f, 0f);

            DrawWhitePieces();
            DrawBlackPieces();

            GL.PopMatrix();
        }

        private void DrawBoard()
        {
            GL.PushMatrix();
            GL.Scale(BOARD_SCALE, BOARD_SCALE, BOARD_SCALE);
            GL.Color4(WHITE_COLOR);
            board.RenderModel();
            GL.PopMatrix();
        }

        private void DrawWhitePieces()
        {
            DrawRooks(WHITE_COLOR, -(QUAD_SIZE * 3 + QUAD_SIZE / 2));
            DrawKnights(WHITE_COLOR, -(QUAD_SIZE * 3 + QUAD_SIZE / 2));
            DrawBishops(WHITE_COLOR, -(QUAD_SIZE * 3 + QUAD_SIZE / 2));
            DrawKing(WHITE_COLOR, -(QUAD_SIZE * 3 + QUAD_SIZE / 2));
            DrawQueen(WHITE_COLOR, -(QUAD_SIZE * 3 + QUAD_SIZE / 2));
            DrawPawns(WHITE_COLOR, -(QUAD_SIZE * 2 + QUAD_SIZE / 2));
        }

        private void DrawBlackPieces()
        {
            DrawRooks(BLACK_COLOR, QUAD_SIZE * 3 + QUAD_SIZE / 2);
            DrawKnights(BLACK_COLOR, QUAD_SIZE * 3 + QUAD_SIZE / 2);
            DrawBishops(BLACK_COLOR, QUAD_SIZE * 3 + QUAD_SIZE / 2);
            DrawKing(BLACK_COLOR, QUAD_SIZE * 3 + QUAD_SIZE / 2);
            DrawQueen(BLACK_COLOR, QUAD_SIZE * 3 + QUAD_SIZE / 2);
            DrawPawns(BLACK_COLOR, QUAD_SIZE * 2 + QUAD_SIZE / 2);
        }

        private void DrawRooks(Color4 color, float yPosition)
        {
            DrawPiece(rook, color, QUAD_SIZE * 3 + QUAD_SIZE / 2, yPosition);
            DrawPiece(rook, color, -(QUAD_SIZE * 3 + QUAD_SIZE / 2), yPosition);
        }

        private void DrawKnights(Color4 color, float yPosition)
        {
            DrawPiece(knight, color, QUAD_SIZE * 2 + QUAD_SIZE / 2, yPosition);
            DrawPiece(knight, color, -(QUAD_SIZE * 2 + QUAD_SIZE / 2), yPosition);
        }

        private void DrawBishops(Color4 color, float yPosition)
        {
            DrawPiece(bishop, color, QUAD_SIZE + QUAD_SIZE / 2, yPosition);
            DrawPiece(bishop, color, -(QUAD_SIZE + QUAD_SIZE / 2), yPosition);
        }

        private void DrawKing(Color4 color, float yPosition)
        {
            DrawPiece(king, color, QUAD_SIZE / 2, yPosition);
        }

        private void DrawQueen(Color4 color, float yPosition)
        {
            DrawPiece(queen, color, -(QUAD_SIZE / 2), yPosition);
        }

        private void DrawPawns(Color4 color, float yPosition)
        {
            for (int i = -4; i < 4; i++)
            {
                DrawPiece(pawn, color, i * QUAD_SIZE + QUAD_SIZE / 2, yPosition);
            }
        }

        private void DrawPiece(Model model, Color4 color, float xPosition, float yPosition)
        {
            GL.PushMatrix();
            GL.Translate(xPosition, yPosition, 0f);
            GL.Color4(color);
            model.RenderModel();
            GL.PopMatrix();
        }
    }
}