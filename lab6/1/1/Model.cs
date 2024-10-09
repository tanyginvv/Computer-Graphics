using Assimp;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;

namespace ChessLab
{
    public class Model
    {
        private Scene scene = new();
        private List<int> displayLists = new();
        private MaterialLoader materialLoader = new();

        public void LoadModel(string filePath)
        {
            using (AssimpContext importer = new AssimpContext())
            {
                scene = importer.ImportFile(filePath, PostProcessSteps.FlipUVs);
            }
            LoadTextures();
            CreateDisplayLists();
        }

        private void CreateDisplayLists()
        {
            int meshCount = scene.MeshCount;
            displayLists.Clear();
            displayLists.Capacity = meshCount;

            for (int i = 0; i < meshCount; i++)
            {
                Mesh mesh = scene.Meshes[i];
                int list = GL.GenLists(1);
                displayLists.Add(list);

                GL.NewList(list, ListMode.Compile);
                materialLoader.ApplyMaterial(scene.Materials[mesh.MaterialIndex], i);

                Vector3[] vertices = AssimpVectorToOpenTKVector3(mesh.Vertices);
                Vector3[] normals = AssimpVectorToOpenTKVector3(mesh.Normals);
                Vector2[] textureCoordinates = AssimpVectorToOpenTKVector2(mesh.TextureCoordinateChannels[0]);

                GL.Begin((BeginMode)(mesh.Faces[0].IndexCount % 3 == 0 ? Assimp.PrimitiveType.Triangle : Assimp.PrimitiveType.Line));

                for (int k = 0; k < vertices.Length; ++k)
                {
                    GL.Normal3(normals[k]);
                    if (scene.Materials[mesh.MaterialIndex].HasTextureDiffuse)
                        GL.TexCoord2(textureCoordinates[k]);
                    GL.Vertex3(vertices[k]);
                }

                GL.End();
                GL.EndList();
            }
        }

        private void LoadTextures()
        {
            materialLoader = new MaterialLoader();
            foreach (var material in scene.Materials)
            {
                materialLoader.LoadMaterialTextures(material);
            }
        }

        public void RenderModel()
        {
            foreach (int list in displayLists)
            {
                GL.CallList(list);
            }
        }

        private Vector3[] AssimpVectorToOpenTKVector3(List<Vector3D> vecArr)
        {
            Vector3[] vector3s = new Vector3[vecArr.Count];

            for (int i = 0; i < vecArr.Count; i++)
            {
                vector3s[i] = new Vector3(vecArr[i].X, vecArr[i].Y, vecArr[i].Z);
            }

            return vector3s;
        }

        private Vector2[] AssimpVectorToOpenTKVector2(List<Vector3D> vecArr)
        {
            Vector2[] vector2s = new Vector2[vecArr.Count];

            for (int i = 0; i < vecArr.Count; i++)
            {
                vector2s[i] = new Vector2(vecArr[i].X, vecArr[i].Y);
            }

            return vector2s;
        }
    }
}