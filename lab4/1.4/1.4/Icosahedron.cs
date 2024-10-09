using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using System;

namespace Icosahedron
{
    public class Icosahedron
    {
        private static readonly float _t = (float)((1.0f + Math.Sqrt(5.0f)) / 2.0f);

        private readonly float[][] _verticles = new float[][]
        {
            new float[]{ -1f, _t, 0f },
            new float[]{ 1f, _t, 0f },
            new float[]{ -1f, -_t, 0f },
            new float[]{ 1f, -_t, 0f },
            new float[]{ 0f, -1f, _t },
            new float[]{ 0f, 1f, _t },
            new float[]{ 0f, -1f, -_t },
            new float[]{ 0f, 1f, -_t },
            new float[]{ _t, 0f, -1f },
            new float[]{ _t, 0f, 1f },
            new float[]{ -_t, 0f, -1f },
            new float[]{ -_t, 0f, 1f }
        };

        private readonly int[][] _sides = new int[][]
        {
            new int[]{ 11, 10, 2 },
            new int[]{ 3, 4, 2 },
            new int[]{ 8, 6, 7 },
            new int[]{ 7, 1, 8 },
            new int[]{ 1, 5, 9 },
            new int[]{ 0, 11, 5 },
            new int[]{ 3, 8, 9 },
            new int[]{ 9, 8, 1 },
            new int[]{ 10, 7, 6 },
            new int[]{ 0, 10, 11 },
            new int[]{ 0, 1, 7 },
            new int[]{ 3, 2, 6 },
            new int[]{ 0, 7, 10 },
            new int[]{ 3, 9, 4 },
            new int[]{ 6, 2, 10 },
            new int[]{ 3, 6, 8 },
            new int[]{ 0, 5, 1 },
            new int[]{ 5, 11, 4 },
            new int[]{ 2, 4, 11 },
            new int[]{ 4, 9, 5 },
        };

        private readonly int[][] _lines = new int[][]
        {
            new int[]{ 0, 11 },
            new int[]{ 0, 5 },
            new int[]{ 5, 11 },
            new int[]{ 0, 1 },
            new int[]{ 1, 5 },
            new int[]{ 1, 7 },
            new int[]{ 0, 7 },
            new int[]{ 0, 10 },
            new int[]{ 7, 10 },
            new int[]{ 0, 11 },
            new int[]{ 10, 11 },
            new int[]{ 1, 9 },
            new int[]{ 5, 9 },
            new int[]{ 4, 5 },
            new int[]{ 4, 11 },
            new int[]{ 2, 11 },
            new int[]{ 2, 10 },
            new int[]{ 6, 10 },
            new int[]{ 6, 7 },
            new int[]{ 1, 8 },
            new int[]{ 7, 8 },
            new int[]{ 3, 4 },
            new int[]{ 3, 9 },
            new int[]{ 4, 9 },
            new int[]{ 2, 3 },
            new int[]{ 2, 4 },
            new int[]{ 3, 6 },
            new int[]{ 2, 6 },
            new int[]{ 3, 8 },
            new int[]{ 6, 8 },
            new int[]{ 8, 9 },
        };


        private Color4[] _sidesColors = new Color4[]
        {
            new Color4(1f, 1f, 1f, 0.8f), // White
            new Color4(1f, 0f, 0f, 0.8f), // Red
            new Color4(0f, 1f, 0f, 0.8f), // Green
            new Color4(0f, 0f, 1f, 0.8f), // Blue
            new Color4(1f, 1f, 0f, 0.8f), // Yellow
            new Color4(0f, 1f, 1f, 0.8f), // Cyan
            new Color4(1f, 0f, 1f, 0.8f), // Magenta
            new Color4(0.5f, 0.8f, 0.5f, 0.8f), // Light Green
            new Color4(0.8f, 0.5f, 0.8f, 0.8f), // Light Magenta
            new Color4(0f, 0f, 0f, 0.8f) // Black
        };

        public void Draw()
        {
            DrawLines();

            GL.Enable(EnableCap.CullFace);

            GL.CullFace(CullFaceMode.Front);
            DrawSides();

            GL.CullFace(CullFaceMode.Back);
            DrawSides();

            GL.Disable(EnableCap.CullFace);
        }

        private void DrawLines()
        {
            GL.Color4(0f, 0f, 0f, 1f);

            GL.Begin(PrimitiveType.Lines);

            foreach (var line in _lines)
            {
                foreach (var vertexIndex in line)
                {
                    GL.Vertex3(_verticles[vertexIndex]);
                }
            }

            GL.End();
        }

        private void DrawSides()
        {
            GL.Begin(PrimitiveType.Triangles);

            GL.GetFloat(GetPName.ModelviewMatrix, out Matrix4 matrix);

            int i = 0;
       
            foreach (var side in _sides)
            {
                GL.Color4(_sidesColors[i % _sidesColors.Length]);

                var arV0 = _verticles[side[0]];
                var arV1 = _verticles[side[1]];
                var arV2 = _verticles[side[2]];

                Vector3 v0 = new Vector3(arV0[0], arV0[1], arV0[2]);
                Vector3 v1 = new Vector3(arV1[0], arV1[1], arV1[2]);
                Vector3 v2 = new Vector3(arV2[0], arV2[1], arV2[2]);

                var normal = Vector3.Cross(
                    v1 - v0,
                    v2 - v0);
                
                normal.Normalize();

                GL.Normal3(normal);

                foreach (var vertexIndex in side)
                {
                    GL.Vertex3(_verticles[vertexIndex]);
                }
                i++;
            }

            GL.End();
        }
    }
}