using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;

namespace ChessLab
{
    internal class ChessWindow : GameWindow
    {
        private Chess _chess;

        public ChessWindow(GameWindowSettings gameWindowSettings, NativeWindowSettings nativeWindowSettings)
            : base(gameWindowSettings, nativeWindowSettings)
        {
        }

        protected override void OnLoad()
        {
            base.OnLoad();

            SetupOpenGLSettings();
            SetupLighting();
            SetupCamera();

            _chess = new Chess();
        }

        private void SetupOpenGLSettings()
        {
            GL.ClearColor(0.5f, 0.5f, 0.5f, 1f);
            GL.Enable(EnableCap.Texture2D);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.ColorMaterial);
            GL.Enable(EnableCap.Normalize);
        }

        private void SetupLighting()
        {
            GL.Enable(EnableCap.Lighting);
            GL.Light(LightName.Light0, LightParameter.Position, new Vector4(1f, 1f, 1f, 0f));
            GL.Light(LightName.Light0, LightParameter.Diffuse, new Vector4(1f, 1f, 1f, 1f));
            GL.Light(LightName.Light0, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1f));
            GL.Light(LightName.Light0, LightParameter.Specular, new Vector4(1f, 1f, 1f, 1f));
            GL.Enable(EnableCap.Light0);
        }

        private void SetupCamera()
        {
            var cameraMatrix = Matrix4.LookAt(
                new Vector3(0f, 350f, 150f), 
                Vector3.Zero,              
                Vector3.UnitY              
            );
            GL.LoadMatrix(ref cameraMatrix);
        }

        protected override void OnResize(ResizeEventArgs e)
        {
            base.OnResize(e);

            GL.Viewport(0, 0, e.Width, e.Height);
            SetupProjectionMatrix(e.Width, e.Height);

            GL.MatrixMode(MatrixMode.Modelview);
            OnRenderFrame(new FrameEventArgs());
        }

        private void SetupProjectionMatrix(int width, int height)
        {
            double aspectRatio = (double)width / height;
            double frustumSize = 2.0;
            double frustumHeight = frustumSize;
            double frustumWidth = frustumHeight * aspectRatio;

            if (frustumWidth < frustumSize)
            {
                frustumWidth = frustumSize;
                frustumHeight = frustumWidth / aspectRatio;
            }

            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            GL.Frustum(
                -frustumWidth * 0.5, frustumWidth * 0.5,  
                -frustumHeight * 0.5, frustumHeight * 0.5, 
                frustumSize * 0.5, frustumSize * 500
            );
        }

        protected override void OnRenderFrame(FrameEventArgs args)
        {
            base.OnRenderFrame(args);

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            _chess.Draw();

            SwapBuffers();
        }
    }
}