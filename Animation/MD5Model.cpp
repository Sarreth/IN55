#include "MD5Model.h"


using namespace std;

MD5Model::MD5Model()
: m_iMD5Version(-1)
, m_iNumJoints(0)
, m_iNumMeshes(0)
, m_bHasAnimation(false)
, m_LocalToWorldMatrix()
{
}

MD5Model::~MD5Model()
{
}

GLuint MD5Model::loadTexture ( string filename, bool useMipMap)
{
        QImage baseTexture, interTexture;
        GLuint finalTexture;

        if (!baseTexture.load ( "pics/whisp.png", "PNG" ))
            qDebug() << "----->ERREUR 02 ; Chargement texture" << QString::fromStdString(filename) << "= FAILED";

        interTexture = QGLWidget::convertToGLFormat ( baseTexture ); //transformation et renversement de l'image
        glGenTextures ( 1, &finalTexture ); //generation de la texture openGL, Ã  ce niveau ont pourrait renvoyer finalTexture

        glBindTexture ( GL_TEXTURE_2D, finalTexture );


        if ( useMipMap )
        {
                gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, interTexture.width(), interTexture.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                                    interTexture.bits() );//creation des 3 mipmaps (adapte a  chaque distance)

                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); //ajout du filtre trilineaire pour le "tres beau rendu"
                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); //filtre lineaire pour longue distance
        }
        else
        {
                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //ajout seulement d'un filtre lineaire
                glTexImage2D ( GL_TEXTURE_2D, 0, 3, interTexture.width(), interTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, interTexture.bits() );
        }

        return finalTexture; //on renvoie la texture

}


void MD5Model::computeQuatW(QQuaternion &quat)
{
    float t = 1.0f - ( quat.x() * quat.x() ) - ( quat.y() * quat.y() ) - ( quat.z() * quat.z() );
    if ( t < 0.0f )
        quat.setScalar(0.0f);
    else
        quat.setScalar(-sqrtf(t));
}

void MD5Model::removeQuotes(string& str )
{
    size_t n;
    while ( ( n = str.find('\"') ) != string::npos ) str.erase(n,1);
}

int MD5Model::getFileLength( std::istream& file )
{
    int pos = file.tellg();
    file.seekg(0, std::ios::end );
    int length = file.tellg();
    // Restore the position of the get pointer
    file.seekg(pos);

    return length;
}

void MD5Model::ignoreLine( std::istream& file, int length )
{
    file.ignore( length, '\n' );
}

bool MD5Model::loadModel( const string &filename )
{
    ifstream file;
    string buffer;

    file.open(filename.c_str(),ios::in);

    if ( file.fail() )
    {
        std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
        return false;
    }

    // store the parent path used for loading images relative to this file.

    size_t found;
    found = filename.find_last_of("/\\");
    string parent_path= filename.substr(0,found);

    string param;
    string junk;   // Read junk from the file

    int fileLength = getFileLength( file );

    m_Joints.clear();
    m_Meshes.clear();

    file >> param;

    while ( !file.eof() )
    {
        if ( param == "MD5Version" )
            file >> m_iMD5Version;
        else if ( param == "commandline" )
            ignoreLine(file, fileLength ); // Ignore the contents of the line
        else if ( param == "numJoints" )
        {
            file >> m_iNumJoints;
            m_Joints.reserve(m_iNumJoints);
        }
        else if ( param == "numMeshes" )
        {
            file >> m_iNumMeshes;
            m_Meshes.reserve(m_iNumMeshes);
        }
        else if ( param == "joints" )
        {
            Joint joint;
            file >> junk; // Read the '{' character
            for ( int i = 0; i < m_iNumJoints; ++i )
            {
                float tempX,tempY,tempZ;
                file >> joint.m_Name >> joint.m_ParentID >> junk
                     >> joint.m_Pos.X >> joint.m_Pos.Y >> joint.m_Pos.Z >> junk >> junk
                     >> tempX >> tempY >> tempZ >> junk;
                joint.m_Orient.setX(tempX);
                joint.m_Orient.setY(tempY);
                joint.m_Orient.setZ(tempZ);
                removeQuotes( joint.m_Name );
                computeQuatW( joint.m_Orient );

                m_Joints.push_back(joint);
                // Ignore everything else on the line up to the end-of-line character.
                ignoreLine( file, fileLength );
            }
            file >> junk; // Read the '}' character
        }
        else if ( param == "mesh" )
        {
            Mesh mesh;
            int numVerts, numTris, numWeights;

            file >> junk; // Read the '{' character
            file >> param;
            while ( param != "}" )  // Read until we get to the '}' character
            {
                if ( param == "shader" )
                {
                    file >> mesh.m_Shader;
                    removeQuotes( mesh.m_Shader );

//                    fs::path shaderPath( mesh.m_Shader );
//                    fs::path texturePath;

                    string shaderPath(mesh.m_Shader);
                    string texturePath;
                    if ( shaderPath.find(parent_path) != string::npos )
                        texturePath = shaderPath;
                    else
//                        texturePath = parent_path / shaderPath;
                        texturePath = parent_path + "/" + shaderPath;


                    texturePath.substr(0,texturePath.find_last_of("."));
                    texturePath+= ".tga" ;

                    mesh.m_TexID = loadTexture( texturePath.c_str(), false );

                    file.ignore(fileLength, '\n' ); // Ignore everything else on the line
                }
                else if ( param == "numverts")
                {
                    file >> numVerts;               // Read in the vertices
                    ignoreLine(file, fileLength);
                    for ( int i = 0; i < numVerts; ++i )
                    {
                        Vertex vert;
                        float tmpX,tmpY;
                        file >> junk >> junk >> junk                    // vert vertIndex (
                            >> tmpX >> tmpY >> junk  //  s t )
                            >> vert.m_StartWeight >> vert.m_WeightCount;
                        vert.m_Tex0.setX(tmpX);
                        vert.m_Tex0.setY(tmpY);
                        ignoreLine(file, fileLength);

                        mesh.m_Verts.push_back(vert);
                        mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
                    }
                }
                else if ( param == "numtris" )
                {
                    file >> numTris;
                    ignoreLine(file, fileLength);
                    for ( int i = 0; i < numTris; ++i )
                    {
                        Triangle tri;
                        file >> junk >> junk >> tri.m_Indices[0] >> tri.m_Indices[1] >> tri.m_Indices[2];

                        ignoreLine( file, fileLength );

                        mesh.m_Tris.push_back(tri);
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[0] );
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[1] );
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[2] );
                    }
                }
                else if ( param == "numweights" )
                {
                    file >> numWeights;
                    ignoreLine( file, fileLength );
                    for ( int i = 0; i < numWeights; ++i )
                    {
                        Weight weight;
                        file >> junk >> junk >> weight.m_JointID >> weight.m_Bias >> junk
                            >> weight.m_Pos.X >> weight.m_Pos.Y >> weight.m_Pos.Z >> junk;

                        ignoreLine( file, fileLength );
                        mesh.m_Weights.push_back(weight);
                    }
                }
                else
                    ignoreLine(file, fileLength);

                file >> param;
            }

            prepareMesh(mesh);
            prepareNormals(mesh);

            m_Meshes.push_back(mesh);

        }

        file >> param;
    }

    return true;
}
/*
bool MD5Model::loadAnim( const std::string& filename )
{
    if ( m_Animation.LoadAnimation( filename ) )
        // Check to make sure the animation is appropriate for this model
        m_bHasAnimation = CheckAnimation( m_Animation );

    return m_bHasAnimation;
}
*/
/*
bool MD5Model::CheckAnimation( const MD5Animation& animation ) const
{
    if ( m_iNumJoints != animation.GetNumJoints() )
        return false;

    // Check to make sure the joints match up
    for ( unsigned int i = 0; i < m_Joints.size(); ++i )
    {
        const Joint& meshJoint = m_Joints[i];
        const MD5Animation::JointInfo& animJoint = animation.GetJointInfo( i );

        if ( meshJoint.m_Name != animJoint.m_Name || meshJoint.m_ParentID != animJoint.m_ParentID )
            return false;
    }

    return true;
}
*/

// Compute the position of the vertices in object local space
// in the skeleton's bind pose
bool MD5Model::prepareMesh( Mesh& mesh )
{
    mesh.m_PositionBuffer.clear();
    mesh.m_Tex2DBuffer.clear();

    // Compute vertex positions
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        Vertex& vert = mesh.m_Verts[i];

        vert.m_Pos = Coord3D();
        vert.m_Normal = Coord3D();

        // Sum the position of the weights
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            Joint& joint = m_Joints[weight.m_JointID];

            // Convert the weight position from Joint local space to object space
            QVector3D vec = joint.m_Orient.rotatedVector(QVector3D(weight.m_Pos.X,weight.m_Pos.Y,weight.m_Pos.Z));
            Coord3D rotPos = Coord3D(vec.x(),vec.y(),vec.z());

            vert.m_Pos += ( joint.m_Pos + rotPos ) * weight.m_Bias;
        }

        mesh.m_PositionBuffer.push_back(vert.m_Pos);
        mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
    }

    return true;
}
/*
bool MD5Model::PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel )
{
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        const Vertex& vert = mesh.m_Verts[i];
        glm::vec3& pos = mesh.m_PositionBuffer[i];
        glm::vec3& normal = mesh.m_NormalBuffer[i];

        pos = glm::vec3(0);
        normal = glm::vec3(0);

        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            const MD5Animation::SkeletonJoint& joint = skel.m_Joints[weight.m_JointID];

            glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;
            pos += ( joint.m_Pos + rotPos ) * weight.m_Bias;

            normal += ( joint.m_Orient * vert.m_Normal ) * weight.m_Bias;
        }
    }
    return true;
}
*/

// Compute the vertex normals in the Mesh's bind pose
bool MD5Model::prepareNormals( Mesh& mesh )
{
    mesh.m_NormalBuffer.clear();

    // Loop through all triangles and calculate the normal of each triangle
    for ( unsigned int i = 0; i < mesh.m_Tris.size(); ++i )
    {
        Coord3D v0 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Pos;
        Coord3D v1 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Pos;
        Coord3D v2 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Pos;

        Coord3D sub1, sub2;

        sub1=v2-v0;
        sub2=v1-v0;

        Coord3D normal = sub1.crossProduct(sub2);

        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Normal += normal;
    }

    // Now normalize all the normals
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        Vertex& vert = mesh.m_Verts[i];

        Coord3D normal = vert.m_Normal.normalize();
        mesh.m_NormalBuffer.push_back( normal );

        // Reset the normal to calculate the bind-pose normal in joint space
        vert.m_Normal = Coord3D();

        // Put the bind-pose normal into joint-local space
        // so the animated normal can be computed faster later
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            const Joint& joint = m_Joints[weight.m_JointID];

            QVector3D vec = joint.m_Orient.rotatedVector(QVector3D(weight.m_Pos.X,weight.m_Pos.Y,weight.m_Pos.Z));

            Coord3D tmp=Coord3D(normal.X*joint.m_Orient.x(),normal.Y*joint.m_Orient.y(),normal.Z*joint.m_Orient.z());
            vert.m_Normal += tmp * weight.m_Bias;
        }
    }

    return true;
}

void MD5Model::update( float fDeltaTime )
{
//    if ( m_bHasAnimation )
//    {
//        m_Animation.Update(fDeltaTime);
//        const MD5Animation::FrameSkeleton& skeleton = m_Animation.GetSkeleton();

//        for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
//        {
//            PrepareMesh( m_Meshes[i], skeleton );
//        }
//    }
}

void MD5Model::render()
{
    glPushMatrix();
    glTranslated ( 40,40,60 );
    glMultMatrixf( m_LocalToWorldMatrix.data() );

    // Render the meshes
    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
        renderMesh( m_Meshes[i] );

//    m_Animation.Render();

    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
        renderNormals( m_Meshes[i] );

    glPopMatrix();
}

void MD5Model::renderMesh( const Mesh& mesh )
{
    glColor3f( 1.0f, 1.0f, 1.0f );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glBindTexture( GL_TEXTURE_2D, mesh.m_TexID );
    glVertexPointer( 3, GL_FLOAT, 0, &(mesh.m_PositionBuffer[0]) );
    glNormalPointer( GL_FLOAT, 0, &(mesh.m_NormalBuffer[0]) );
    glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh.m_Tex2DBuffer[0]) );

    glDrawElements( GL_TRIANGLES, mesh.m_IndexBuffer.size(), GL_UNSIGNED_INT, &(mesh.m_IndexBuffer[0]) );

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void MD5Model::renderNormals(  const Mesh& mesh )
{

    glPushAttrib( GL_ENABLE_BIT );
    glDisable( GL_LIGHTING );

    glColor3f( 1.0f, 1.0f, 0.0f );// Yellow

    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < mesh.m_PositionBuffer.size(); ++i )
        {
            Coord3D p0 = mesh.m_PositionBuffer[i];
            Coord3D p1 = ( mesh.m_PositionBuffer[i] + mesh.m_NormalBuffer[i] );

            glVertex3f( p0.X,p0.Y,p0.Z);
            glVertex3f( p1.X,p1.Y,p1.Z );
        }
    }
    glEnd();

    glPopAttrib();
}

void MD5Model::renderSkeleton( const JointList& joints )
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
            glVertex3f( joints[i].m_Pos.X,joints[i].m_Pos.Y,joints[i].m_Pos.Z );
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            const Joint& j0 = joints[i];
            if ( j0.m_ParentID != -1 )
            {
                const Joint& j1 = joints[j0.m_ParentID];
                glVertex3f( j0.m_Pos.X,j0.m_Pos.Y,j0.m_Pos.Z );
                glVertex3f( j1.m_Pos.X,j1.m_Pos.Y,j1.m_Pos.Z );
            }
        }
    }
    glEnd();

    glPopAttrib();

}
