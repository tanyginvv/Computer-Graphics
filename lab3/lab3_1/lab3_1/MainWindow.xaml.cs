using System.Windows;
using SharpGL;
using SharpGL.WPF;

namespace lab3_1
{
    public partial class MainWindow : Window
    {
        //график сохраняет пропорции при ресайзе
        //отдельный класс для осей и функции
        private const float minX = -2f;
        private const float maxX = 3f;
        private const float minY = -5f;
        private const float maxY = 5f;
        private const float step = 0.1f;

        public MainWindow()
        {
            InitializeComponent();
            openGLControl.OpenGLDraw += OpenGLControl_OpenGLDraw;
            openGLControl.OpenGLInitialized += OpenGLControl_OpenGLInitialized;
        }

        private void OpenGLControl_OpenGLInitialized(object sender, OpenGLRoutedEventArgs args)
        {
            var gl = openGLControl.OpenGL;
            gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        private void OpenGLControl_OpenGLDraw(object sender, OpenGLRoutedEventArgs args)
        {
            var gl = openGLControl.OpenGL;
            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
            gl.LoadIdentity();

            // Определяем размеры окна
            var width = openGLControl.ActualWidth;
            var height = openGLControl.ActualHeight;

            // Определяем масштаб для вписывания графика в пределы от -1 до 1 по осям x и y
            float scaleX = (float)(100 / width);
            float scaleY = (float)(100 / height);
            float scale = Math.Min(scaleX, scaleY);

            gl.MatrixMode(OpenGL.GL_PROJECTION);
            gl.LoadIdentity();

            gl.Scale(scale, scale, 1); // Применяем масштабирование

            DrawAxes(gl);
            DrawGraphic(gl);

            gl.Flush();
        }


        private void DrawGraphic(OpenGL gl)
        {
            gl.Begin(OpenGL.GL_LINE_STRIP);
            gl.Color(1.0f, 0.0f, 0.0f); 

            for (float x = minX; x <= maxX; x += step)
            {
                float y = 2 * x * x - 3 * x - 8;
                float normalizedX = 2 * (x - minX) / (maxX - minX) - 1; // Преобразуем x в относительные координаты
                float normalizedY = 2 * (y - minY) / (maxY - minY) - 1; // Преобразуем y в относительные координаты
                gl.Vertex(normalizedX, normalizedY, 0);
            }

            gl.End();
        }
        private void DrawAxes(OpenGL gl)
        {
            gl.Begin(OpenGL.GL_LINES);
            gl.Color(0.0f, 1.0f, 0.0f); 

            // Ось X
            gl.Vertex(minX, 0, 0);
            gl.Vertex(maxX, 0, 0);

            // Ось Y
            gl.Vertex(0, minY, 0);
            gl.Vertex(0, maxY, 0);

            gl.End();
        }
    }
}