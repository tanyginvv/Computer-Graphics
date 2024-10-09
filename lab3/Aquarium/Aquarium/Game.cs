using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;

namespace Aquarium
{
    public class Bubble
    {
        public Vector2 Position;
        public Vector2 Speed;
        public Vector2 Direction;

        public Bubble(Vector2 position, Vector2 speed, Vector2 direction)
        {
            Position = position;
            Speed = speed;
            Direction = direction;
        }
    }

    public class Game : GameWindow
    {
        private float speed1 = 0.1f;
        private float speed2 = 0.1f;
        private float speed4 = 0.1f;
        private float speed3 = 0.1f;
        private float speed5 = 0.1f;
        private float fish1X;
        private float fish2X;
        private float fish3X;
        private float fish4X;
        private float fish5X;
        private readonly List<Bubble> bubbles = new List<Bubble>();

        public Game(GameWindowSettings gameWindowSettings, NativeWindowSettings nativeWindowSettings)
            : base(gameWindowSettings, nativeWindowSettings)
        {
        }

        protected override void OnLoad()
        {
            base.OnLoad();
            GL.ClearColor(Color4.White);
        }

        protected override void OnResize(ResizeEventArgs e)
        {
            base.OnResize(e);
            GL.Viewport(0, 0, Size.X, Size.Y);
            AdjustProjection();
            SwapBuffers();
        }

        protected override void OnUpdateFrame(FrameEventArgs args)
        {
            base.OnUpdateFrame(args);
            UpdateFishPositions((float)args.Time);
            UpdateBubbles((float)args.Time);
        }

        void UpdateFishPositions(float deltaTime)
        {
            fish1X += speed1 * deltaTime;
            fish2X += speed2 * deltaTime;
            fish3X += speed3 * deltaTime;
            fish4X += speed4 * deltaTime;
            fish5X += speed5 * deltaTime;
        }

        void UpdateBubbles(float deltaTime)
        {
            foreach (var bubble in bubbles)
            {
                if (bubble.Direction.Y > 0)
                {
                    bubble.Position += bubble.Speed * bubble.Direction * deltaTime;

                    if (bubble.Position.Y > 600)
                    {
                        bubbles.Remove(bubble);
                        break;
                    }
                }
            }
        }

        protected override void OnRenderFrame(FrameEventArgs args)
        {
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            DrawAquarium();
            DrawBubbles();
            SwapBuffers();
            base.OnRenderFrame(args);
        }

        void DrawBubbles()
        {
            foreach (var bubble in bubbles)
            {
                DrawBubble(bubble.Position.X, bubble.Position.Y);
            }
        }

        void DrawBubble(float x, float y)
        {
            GL.PushMatrix();
            GL.Translate(x, y, 0);

            GL.Begin(PrimitiveType.TriangleFan);
            GL.Color3(1.0f, 1.0f, 1.0f); // Белый цвет
            const int numSegments = 20;
            const float radius = 10.0f;

            GL.Vertex2(0, 0); // Центр пузыря

            for (int i = 0; i <= numSegments; i++)
            {
                float theta = (float)(i * 2 * Math.PI / numSegments);
                float bubbleX = radius * MathF.Cos(theta);
                float bubbleY = radius * MathF.Sin(theta);
                GL.Vertex2(bubbleX, bubbleY);
            }

            GL.End();

            GL.PopMatrix();
        }

        protected override void OnUnload()
        {
            base.OnUnload();
            fish1X = 0.0f;
            fish2X = 0.0f;
            fish3X = 0.0f;
            fish4X = 0.0f;
            fish5X = 0.0f;

            GL.ClearColor(Color4.White);
        }

        void AdjustProjection()
        {
            float aspectRatio = (float)Size.X / Size.Y;
            Matrix4 projection = Matrix4.CreateOrthographic(1600 * aspectRatio, 1200, -1, 1);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);
        }

        void DrawEllipse(float x, float y, float radiusX, float radiusY)
        {
            GL.Begin(PrimitiveType.Polygon);
            for (int i = 0; i < 360; i++)
            {
                float degInRad = i * (float)Math.PI / 180;
                GL.Vertex2(x + Math.Cos(degInRad) * radiusX, y + Math.Sin(degInRad) * radiusY);
            }
            GL.End();
        }
        void DrawAquarium()
        {
            // Отрисовка воды
            GL.Begin(PrimitiveType.Quads);
            GL.Color3(0.6f, 0.77f, 0.97f);
            GL.Vertex2(-800.0f, -600.0f);
            GL.Vertex2(800.0f, -600.0f);
            GL.Vertex2(800.0f, 600.0f);
            GL.Vertex2(-800.0f, 600.0f);
            GL.End();


            // Отрисовка растений
            DrawPlant(-500.0f, -600.0f, 60.0f, 450.0f);
            DrawPlant(-100.0f, -600.0f, 50.0f, 250.0f);
            DrawPlant(100.0f, -600.0f, 60.0f, 300.0f);
            DrawPlant(-500.0f, -600.0f, 60.0f, 450.0f);
            DrawPlant(300.0f, -600.0f, 20.0f, 150.0f);
            DrawPlant(500.0f, -600.0f, 60.0f, 550.0f);
            DrawPlant(-400.0f, -600.0f, 100.0f, 750.0f);
            DrawPlant(-700.0f, -600.0f, 80.0f, 350.0f);

            // Отрисовка камней
            DrawStone2(-400.0f, -600.0f, 100.0f, 100.0f);
            DrawStone4(-240.0f, -600.0f, 50.0f, 50.0f);
            DrawStone2(100.0f, -600.0f, 60.0f, 30.0f);
            DrawStone2(600.0f, -600.0f, 40.0f, 20.0f);
            DrawStone1(60.0f, -600.0f, 60.0f, 40.0f);
            DrawStone3(-550.0f, -600.0f, 50.0f, 50.0f);
            DrawStone4(200.0f, -600.0f, 40.0f, 40.0f);
            DrawStone1(500.0f, -600.0f, 70.0f, 50.0f);
            DrawStone5(440.0f, -600.0f, 40.0f, 40.0f);
            DrawStone5(630.0f, -600.0f, 40.0f, 40.0f);


            // Отрисовка рыб
            DrawFish2(370.0f, -100.0f, 4, 120.0f, 80.0f, ref speed2);
            DrawFish5(250.0f, 300.0f, 5, 150.0f, 40.0f,  ref speed5);
          //  DrawFish1(-600.0f, 400.0f, 7, 150.0f, 40.0f, ref  speed1);
            DrawFish4(-170.0f, 50.0f, 9, 120.0f, 80.0f, ref speed4);
            DrawFish3(-400.0f, -200.0f, 1, ref speed3);

        }
        void DrawPlant(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.Quads);
            GL.Color3(0.0f, 0.5f, 0.0f);
            float offset = 10.0f; 

            for (float i = 0; i < width; i += offset)
            {
                GL.Vertex2(x + i, y);
                GL.Vertex2(x + i + offset, y);
                GL.Vertex2(x + i + offset, y + height * (float)Math.Sin((i + offset / 2) * Math.PI / width));
                GL.Vertex2(x + i, y + height * (float)Math.Sin((i + offset / 2) * Math.PI / width));
            }

            GL.End();
        }


        void DrawFish1(float x, float y, int color, float rx, float ry, ref float speed)
        {
            GL.PushMatrix();

            if (fish1X >= 400.0f)
            {
                fish1X = -1450;
            }

            if (fish1X >= -551 && fish1X <= -550)
            {
                bubbles.Add(new Bubble(new Vector2(x + 420, y), new Vector2(0, 50), new Vector2(0, 1)));
            }

            fish1X += speed;

            GL.Translate(-fish1X, 0.0f, 0.0f);

            // Тело
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.79f, 0.97f, 0.08f);
            DrawEllipse(x, y, rx, ry);
            GL.End();

            // Хвост
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.79f, 0.97f, 0.08f);
            GL.Vertex2(x + rx, y);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 50.0, y + 50.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 100.0, y + 20.0);
            GL.Vertex2(x + rx + 70.0, y);
            GL.Vertex2(x + rx + 100, y - 20.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 50.0, y - 50.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx, y);
            GL.End();
            //выделить код рисования рыб в класс
            //избавиться от передачи по ссылке

            // Глаз
            DrawEllipse(x - 50.0f, y + 20.0f, ry / 2, ry / 2);
            GL.Color3(1.0f, 1.0f, 1.0f);
            DrawEllipse(x - 40.0f, y + 30.0f, ry / 8, ry / 8);

            GL.PopMatrix();
        }

        void DrawFish2(float x, float y, int color, float rx, float ry, ref float speed)
        {
            GL.PushMatrix();
            GL.Translate(x, y, 0.0f);
            GL.Rotate(180, Vector3.UnitY);
            if (fish2X >= 900.0f)
            {
                fish2X = -850.0f;
            }

            if (fish2X >= 0 && fish2X <= 1)
            {
                bubbles.Add(new Bubble(new Vector2(fish2X + 150.0f , y - 50.0f), new Vector2(0, 50), new Vector2(0, 1)));
            }

            fish2X += speed;
            GL.Translate(-fish2X, 0.0f, 0.0f);
            // Тело
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.0f, 0.5f, 0.5f); // Темно-зеленый цве
            DrawEllipse(x, y, rx, ry);
            GL.End();

            // Хвост
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.0f, 0.5f, 0.5f); // Темно-зеленый цвет

            DrawEllipse(x + rx - 10, y + 15, ry, ry);
            DrawEllipse(x + rx - 10, y - 15, ry, ry);
            DrawEllipse(x + rx + 10, y, ry, ry);
            GL.End();

            // Глаз
            DrawEllipse(x - 50.0f, y + 20.0f, ry / 2, ry / 2);
            GL.Color3(0.0f, 0.0f, 0.0f);
            DrawEllipse(x - 40.0f, y + 30.0f, ry / 8, ry / 8);
            GL.PopMatrix();
        }
       
        void DrawFish3(float x, float y, int color, ref float speed)
        {
            GL.PushMatrix();
            if (fish3X > 500.0f)
            {
                fish3X = -1200.0f;
            }

            if (fish3X >= 100 && fish3X <= 101)
            {
                bubbles.Add(new Bubble(new Vector2(x - 100.0f , y - 100.0f), new Vector2(0, 50), new Vector2(0, 1)));
            }

            fish3X += speed;

            GL.Translate(-fish3X, y, 0.0f);
            // Тело
            GL.Begin(PrimitiveType.Polygon);
            GL.Vertex2(x - 80.0f, y);
            GL.Vertex2(x + 80.0f, y + 150.0f);
            GL.Vertex2(x + 80.0f, y - 150.0f);
            GL.Vertex2(x - 80.0f, y);
            GL.End();

            // Хвост
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(color * 0.1f, color * 0.2f, color * 0.3f);
            GL.Vertex2(x + 50.0f, y);
            GL.Color3(color * 0.01f, color * 0.02f, color * 0.03f);
            GL.Vertex2(x + 110.0f, y + 50.0f);
            GL.Color3(color * 0.001f, color * 0.002f, color * 0.003f);
            GL.Vertex2(x + 160.0f, y + 20.0f);
            GL.Vertex2(x + 130.0f, y);
            GL.Vertex2(x + 160.0f, y - 20.0f);
            GL.Color3(color * 0.01f, color * 0.02f, color * 0.03f);
            GL.Vertex2(x + 110.0f, y - 50.0f);
            GL.Color3(color * 0.1f, color * 0.2f, color * 0.3f);
            GL.Vertex2(x + 50.0f, y);
            GL.End();

            // Глаз
            DrawEllipse(x - 50.0f, y + 20.0f, 20.0f, 20.0f);
            GL.Color3(1.0f, 1.0f, 1.0f);
            DrawEllipse(x - 40.0f, y + 30.0f, 5.0f, 5.0f);
            
            GL.PopMatrix();
        }

        void DrawFish4(float x, float y, int color, float rx, float ry, ref float speed)
        {
            GL.PushMatrix();
            if (fish4X >= 850.0f)
            {
                fish4X = -950.0f;   
            }

            if (fish4X >= 50 && fish4X <= 51)
            {
                bubbles.Add(new Bubble(new Vector2(fish4X - 300.0f, y), new Vector2(0, 50), new Vector2(0, 1)));
            }

            fish4X += speed;
            GL.Translate(-fish4X, 0.0f, 0.0f);
            // Тело
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.97f, 0.72f, 0.08f); // Темно-зеленый цве
            DrawEllipse(x, y, rx, ry);
            GL.End();

            // Хвост
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.97f, 0.72f, 0.08f); // Темно-зеленый цвет

            DrawEllipse(x + rx - 10, y + 15, ry, ry);
            DrawEllipse(x + rx - 10, y - 15, ry, ry);
            DrawEllipse(x + rx + 10, y, ry, ry);
            GL.End();

            // Глаз
            DrawEllipse(x - 50.0f, y + 20.0f, ry / 2, ry / 2);
            GL.Color3(0.0f, 0.0f, 0.0f);
            DrawEllipse(x - 40.0f, y + 30.0f, ry / 8, ry / 8);
            GL.PopMatrix();
        }
        void DrawFish5(float x, float y, int color, float rx, float ry,  ref float speed)
        {
            GL.PushMatrix();

            if (fish5X > 1300.0f)
            {
                fish5X = -600.0f;
            }

            if (fish5X <= -100 && fish5X >= -102)
            {
                bubbles.Add(new Bubble(new Vector2(fish5X + 280.0f, y), new Vector2(0, 50), new Vector2(0, 1)));
            }

            fish5X += speed;

            GL.Translate(-fish5X, 0.0f, 0.0f);

            // Тело
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.97f, 0.08f, 0.78f);
            DrawEllipse(x, y, rx, ry);
            GL.End();

            // Хвост
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.97f, 0.08f, 0.78f);
            GL.Vertex2(x + rx, y);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 50.0, y + 50.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 100.0, y + 20.0);
            GL.Vertex2(x + rx + 70.0, y);
            GL.Vertex2(x + rx + 100, y - 20.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx + 50.0, y - 50.0);
            GL.Color3(0.0f, 0.0f, 1f);
            GL.Vertex2(x + rx, y);
            GL.End();

            // Глаз
            DrawEllipse(x - 50.0f, y + 20.0f, ry / 2, ry / 2);
            GL.Color3(1.0f, 1.0f, 1.0f);
            DrawEllipse(x - 40.0f, y + 30.0f, ry / 8, ry / 8);

            GL.PopMatrix();
        }


        void DrawStone1(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.3f, 0.1f, 0.003f);
            GL.Vertex2(x - width / 2, y);
            GL.Color3(0.7f, 0.4f, 0.05f);
            GL.Vertex2(x - width / 4, y + height / 2);
            GL.Vertex2(x + width / 4, y + height / 3);
            GL.Vertex2(x + width / 2, y);
            GL.End();
        }

        void DrawStone2(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.Polygon);
            GL.Color3(0.3f, 0.1f, 0.003f);
            DrawEllipse(x, y, width, height);
            GL.End();
        }

        void DrawStone3(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.TriangleFan);
            GL.Color3(0.5f, 0.5f, 0);
            GL.Vertex2(x, y);
            for (int i = 0; i <= 6; i++)
            {
                GL.Vertex2(x + (width * 2 * Math.Cos(i * 2 * Math.PI / 6)), y + (height * 2 * Math.Sin(i * 2 * Math.PI / 6)));
            }
            GL.End();
        }

        void DrawStone4(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.TriangleFan);
            GL.Color3(0.5f, 0.25f, 0.1f);
            GL.Vertex2(x, y);
            for (int i = 0; i <= 32; i++)
            {
                GL.Vertex2(x + (width * 2 * Math.Cos(i * 2 * Math.PI / 32)), y + (height * 2 * Math.Sin(i * 2 * Math.PI / 32)));
            }
            GL.End();

            GL.Begin(PrimitiveType.TriangleFan);
            float newX = x + width * 2;
            GL.Color3(1, 0.9f, 0.8f);
            GL.Vertex2(newX, y / 2);
            for (int i = 0; i <= 12; i++)
            {
                GL.Vertex2(newX + (height * Math.Cos(i * 2 * Math.PI / 12)), y + (height * Math.Sin(i * 2 * Math.PI / 12)));
            }
            GL.End();
        }

        void DrawStone5(float x, float y, float width, float height)
        {
            GL.Begin(PrimitiveType.TriangleFan);
            GL.Color3(0.5f, 0.25f, 0.1f);
            GL.Vertex2(x, y);
            for (int i = 0; i <= 32; i++)
            {
                GL.Vertex2(x + (width * 2 * Math.Cos(i * 2 * Math.PI / 32)), y + (height * 2 * Math.Sin(i * 2 * Math.PI / 32)));
            }
            GL.End();

            GL.Begin(PrimitiveType.TriangleFan);
            float newX = x + width * 2;
            GL.Color3(1, 0.9f, 0.8f);
            GL.Vertex2(newX, y / 2);
            for (int i = 0; i <= 12; i++)
            {
                GL.Vertex2(newX + (height * Math.Cos(i * 2 * Math.PI / 12)), y + (height * Math.Sin(i * 2 * Math.PI / 12)));
            }
            GL.End();
        }
    }
}