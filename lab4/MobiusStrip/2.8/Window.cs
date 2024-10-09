using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using OpenTK.Windowing.GraphicsLibraryFramework;

namespace MobiusStrip
{
    public class Window : GameWindow
    {

        private bool _leftButtonPressed = false;
        private float _mouseX = 0;
        private float _mouseY = 0;

        private readonly MobiusStrip shape = new MobiusStrip();



        public Window(GameWindowSettings gameWindowSettings, NativeWindowSettings nativeWindowSettings)
            : base(gameWindowSettings, nativeWindowSettings)
        {
        }

        protected override void OnLoad()
        {

            base.OnLoad();

            GL.ClearColor(0f, 0f, 0f, 1f);

            GL.Enable(EnableCap.DepthTest);

            GL.Light(LightName.Light1, LightParameter.Position, new Vector4(10f, 10f, 10f, 0f));
            GL.Light(LightName.Light1, LightParameter.Diffuse, new Vector4(1f, 1f, 1f, 1f));
            GL.Light(LightName.Light1, LightParameter.Ambient, new Vector4(0.2f, 0.2f, 0.2f, 1f));
            GL.Light(LightName.Light1, LightParameter.Specular, new Vector4(1f, 1f, 1f, 1f));


            GL.Enable(EnableCap.Lighting);
            GL.Enable(EnableCap.Light1);

            GL.Enable(EnableCap.ColorMaterial);
            
            GL.LoadIdentity();
            var matrix = Matrix4.LookAt(
                0f, 0f, 5f,
                0f, 0f, 0f,
                0, 1, 0);
            GL.LoadMatrix(ref matrix);
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            if (e.Button == MouseButton.Left)
            {
                _leftButtonPressed = true;

                _mouseX = MousePosition.X;
                _mouseY = MousePosition.Y;
            }


            base.OnMouseDown(e);
        }

        protected override void OnMouseMove(MouseMoveEventArgs e)
        {
            if (!_leftButtonPressed) return;
           
            float dx = e.X - _mouseX;
            float dy = e.Y - _mouseY;

            float rotateX = dy * 180 / this.Size.X;
            float rotateY = dx * 180 / this.Size.Y;
            RotateCamera(rotateX, rotateY);

            _mouseX = e.X;
            _mouseY = e.Y;

            base.OnMouseMove(e);

            OnRenderFrame(new FrameEventArgs());
        }

        private void RotateCamera(float x, float y)
        {
            GL.MatrixMode(MatrixMode.Modelview);

            GL.GetFloat(GetPName.ModelviewMatrix, out Matrix4 modelView);

            Vector3 xAxis = new Vector3(modelView[0, 0], modelView[1, 0], modelView[2, 0]);
            Vector3 yAxis = new Vector3(modelView[0, 1], modelView[1, 1], modelView[2, 1]);

            GL.Rotate(x, xAxis);
            GL.Rotate(y, yAxis);
            NormalizeModelViewMatrix();
        }

        private void NormalizeModelViewMatrix()
        {
            Matrix4 modelView;
            GL.GetFloat(GetPName.ModelviewMatrix, out modelView);
          
            Vector3 xAxis = new Vector3(modelView[0, 0], modelView[1, 0], modelView[2, 0]);
            xAxis.Normalize();
            Vector3 yAxis = new Vector3(modelView[0, 1], modelView[1, 1], modelView[2, 1]);
            yAxis.Normalize();

            Vector3 zAxis = Vector3.Cross(xAxis, yAxis);

            zAxis.Normalize();
          
            xAxis = Vector3.Cross(yAxis, zAxis);
            xAxis.Normalize();
            yAxis = Vector3.Cross(zAxis, xAxis);
            yAxis.Normalize();

            modelView[0, 0] = xAxis.X;
            modelView[1, 0] = xAxis.Y;
            modelView[2, 0] = xAxis.Z;

            modelView[0, 1] = yAxis.X;
            modelView[1, 1] = yAxis.Y; 
            modelView[2, 1] = yAxis.Z;

            modelView[0, 2] = zAxis.X;
            modelView[1, 2] = zAxis.Y;
            modelView[2, 2] = zAxis.Z;

            GL.LoadMatrix(ref modelView);
        }


        protected override void OnMouseUp(MouseButtonEventArgs e)
        {
            _leftButtonPressed = false;
            base.OnMouseUp(e);
        }

        protected override void OnMouseLeave()
        {
            _leftButtonPressed = false;
            base.OnMouseLeave();
        }

        protected override void OnResize(ResizeEventArgs e)
        {
            int width = e.Width;
            int height = e.Height;

            GL.Viewport(0, 0, width, height);

            SetupProjectionMatrix(width, height);


            GL.MatrixMode(MatrixMode.Modelview);
            base.OnResize(e);

            OnRenderFrame(new FrameEventArgs());
        }

        void SetupProjectionMatrix(int width, int height)
        {
            double frustumSize = 2;
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            double aspectRatio = ((double)width) / ((double)height);
            double frustumHeight = frustumSize;
            double frustumWidth = frustumHeight * aspectRatio;
            if (frustumWidth < frustumSize && (aspectRatio != 0))
            {
                frustumWidth = frustumSize;
                frustumHeight = frustumWidth / aspectRatio;
            }
            GL.Frustum(
                -frustumWidth * 0.5, frustumWidth * 0.5, // left, right
                -frustumHeight * 0.5, frustumHeight * 0.5, // top, bottom
                frustumSize * 0.5, frustumSize * 30 // znear, zfar
                );
        }

        protected override void OnRenderFrame(FrameEventArgs args)
        {
            GL.MatrixMode(MatrixMode.Modelview);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            GL.LineWidth(2);

            shape.Draw();

            SwapBuffers();
            base.OnRenderFrame(args);
        }

        protected override void OnUpdateFrame(FrameEventArgs args)
        {
            base.OnUpdateFrame(args);
            this.OnRefresh();
        }

        protected override void OnUnload()
        {
            base.OnUnload();
        }
    }
}