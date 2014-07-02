#ifndef MD5MODEL_H
#define MD5MODEL_H

#include "CameraLibre.h"

#include "MD5Animation.h"

class MD5Model
{

public:

    typedef std::vector<QVector3D> PositionBuffer;
    typedef std::vector<QVector3D> NormalBuffer;
    typedef std::vector<QPoint> Tex2DBuffer;
    typedef std::vector<GLuint> IndexBuffer;

    struct Vertex
    {
        QVector3D     m_Pos;
        QVector3D     m_Normal;
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
        QVector3D         m_Pos;
    };
    typedef std::vector<Weight> WeightList;

    struct Joint
    {
        std::string     m_Name;
        int             m_ParentID;
        QVector3D         m_Pos;
        QQuaternion     m_Orient;

        Joint& operator=(const Joint& j){
            m_Name = j.m_Name;
            m_Orient = j.m_Orient;
            m_ParentID = j.m_ParentID;
            m_Pos = j.m_Pos;
        }
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


    MD5Model();
    MD5Model(const MD5Model& model);
    virtual ~MD5Model();

    bool loadModel( const std::string& filename );
    bool loadAnim( const std::string& filename );
    void clearAnimation();
    void update( float fDeltaTime );
    void render();

    void update();
    int getIndexJointByName(const std::string& name);
    void computeQuatW(QQuaternion& quat);
    void removeQuotes(std::string& str );
    int getFileLength( std::istream& file );
    void ignoreLine( std::istream& file, int length );
    GLuint loadTexture(std::string file, bool mipmap);
    MeshList getMeshList();
    JointList getJointList();
    void setJointList(JointList& joints);
    void setJoint(Joint joint, int index);
    void resizeSkelton(int);
    JointList getRestPosition();
    void setIsWalking(bool b);
    void setPosition( QVector3D position);
    void translation( double coeff);
    MD5Animation& getAnimation();




protected:

    // Prepare the mesh for rendering
    // Compute vertex positions and normals
    bool prepareMesh( Mesh& mesh );
    bool prepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel );
    //  bool PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel );
    bool prepareNormals( Mesh& mesh);

    // Render a single mesh of the model
    void renderMesh( const Mesh& mesh );
    void renderNormals(const Mesh& mesh);

    // Draw the skeleton of the mesh for debugging purposes.
    void renderSkeleton(const JointList& joints , std::string jointName);

     bool CheckAnimation( const MD5Animation& animation ) const;
     void afficheSkelton(MD5Model::JointList& jl);



private:

    int                 m_iMD5Version;
    int                 m_iNumJoints;
    int                 m_iNumMeshes;
    bool                m_bHasAnimation;
    bool                m_isWalking;
    JointList           m_Joints;
    MeshList            m_Meshes;
    MD5Animation        m_Animation;
    QMatrix4x4          m_LocalToWorldMatrix;
    QVector3D           m_Position;
};

#endif // MD5MODEL_H
