#ifndef MD5MODEL_H
#define MD5MODEL_H

#include "CameraLibre.h"

class MD5Model
{
public:
    MD5Model();
    virtual ~MD5Model();

    bool loadModel( const std::string& filename );
    void update( float fDeltaTime );
    void render();

    void computeQuatW(QQuaternion& quat);
    void removeQuotes(std::string& str );
    int getFileLength( std::istream& file );
    void ignoreLine( std::istream& file, int length );
    GLuint loadTexture(std::string file, bool mipmap);

protected:
    typedef std::vector<Coord3D> PositionBuffer;
    typedef std::vector<Coord3D> NormalBuffer;
    typedef std::vector<QPoint> Tex2DBuffer;
    typedef std::vector<GLuint> IndexBuffer;

    struct Vertex
    {
        Coord3D     m_Pos;
        Coord3D     m_Normal;
        QPoint      m_Tex0;
        int         m_StartWeight;
        int         m_WeightCount;
    };
    typedef std::vector<Vertex> VertexList;

    struct Triangle
    {
        int             m_Indices[3];
    };
    typedef std::vector<Triangle> TriangleList;

    struct Weight
    {
        int             m_JointID;
        float           m_Bias;
        Coord3D         m_Pos;
    };
    typedef std::vector<Weight> WeightList;

    struct Joint
    {
        std::string     m_Name;
        int             m_ParentID;
        Coord3D         m_Pos;
        QQuaternion     m_Orient;
    };
    typedef std::vector<Joint> JointList;

    struct Mesh
    {
        std::string     m_Shader;
        // This vertex list stores the vertices in the bind pose.
        VertexList      m_Verts;
        TriangleList    m_Tris;
        WeightList      m_Weights;

        // A texture ID for the material
        GLuint          m_TexID;

        // These buffers are used for rendering the animated mesh
        PositionBuffer  m_PositionBuffer;   // Vertex position stream
        NormalBuffer    m_NormalBuffer;     // Vertex normals stream
        Tex2DBuffer     m_Tex2DBuffer;      // Texture coordinate set
        IndexBuffer     m_IndexBuffer;      // Vertex index buffer
    };
    typedef std::vector<Mesh> MeshList;

    // Prepare the mesh for rendering
    // Compute vertex positions and normals
    bool prepareMesh( Mesh& mesh );
//    bool PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel );
    bool prepareNormals( Mesh& mesh );

    // Render a single mesh of the model
    void renderMesh( const Mesh& mesh );
    void renderNormals( const Mesh& mesh );

    // Draw the skeleton of the mesh for debugging purposes.
    void renderSkeleton( const JointList& joints );

//    bool CheckAnimation( const MD5Animation& animation ) const;
private:

    int                 m_iMD5Version;
    int                 m_iNumJoints;
    int                 m_iNumMeshes;

    bool                m_bHasAnimation;

    JointList           m_Joints;
    MeshList            m_Meshes;

//    MD5Animation        m_Animation;

    QMatrix4x4          m_LocalToWorldMatrix;
};

#endif // MD5MODEL_H
