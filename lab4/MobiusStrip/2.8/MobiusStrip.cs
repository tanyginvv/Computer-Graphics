using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using System;

namespace MobiusStrip
{
    public class MobiusStrip
    {
        private const float ScaleFactor = 1.5f;

        private void SetColorByCoords(Vector3 p)
        {
            p.Normalize();

            float r = Math.Abs(p.X); 
            float g = Math.Abs(p.Y); 
            float b = Math.Abs(p.Z); 

            GL.Color3(r, g, b);
        }

        private void SetVertex(double u, double v)
        {
            float x = (float)((1 + v / 2 * Math.Cos(u / 2)) * Math.Cos(u)) * ScaleFactor;
            float y = (float)((1 + v / 2 * Math.Cos(u / 2)) * Math.Sin(u)) * ScaleFactor;
            float z = (float)(v / 2 * Math.Sin(u / 2)) * ScaleFactor;

            SetColorByCoords(new Vector3(x, y, z));

            GL.Vertex3(x, y, z);
        }

        public void Draw()
        {
            GL.Begin(PrimitiveType.QuadStrip);

            double u, v;
            for (v = -1; v <= 1; v += 0.05)
            {
                for (u = 0; u < 2 * Math.PI + 0.05; u += 0.05)
                {
                    SetVertex(u, v);
                    SetVertex(u, v + 0.05);
                }
            }

            GL.End();
        }
    }
}