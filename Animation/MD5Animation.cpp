
#include "MD5Animation.h"
using namespace std;

MD5Animation::MD5Animation()
: m_iMD5Version( 0 )
, m_iNumFrames( 0 )
, m_iNumJoints( 0 )
, m_iFramRate( 0 )
, m_iNumAnimatedComponents( 0 )
, m_fAnimDuration( 0 )
, m_fFrameDuration( 0 )
,m_continuous(false)
, m_fAnimTime( 0 )
{

}

MD5Animation::~MD5Animation()
{}


void MD5Animation::clearAnimation(){
    m_JointInfos.clear();
    m_Bounds.clear();
    m_BaseFrames.clear();
    m_Frames.clear();
    m_AnimatedSkeleton.m_Joints.clear();
    m_iNumFrames = 0;
    m_Skeletons.clear();
}

void MD5Animation::setContinuous( bool b)
{
    m_continuous = b;
}

bool MD5Animation::loadAnimation( const std::string& filename )
{
    ifstream file;
    //string buffer;

    file.open(filename.c_str(),ios::in);

    if ( file.fail() )
    {
        std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
        return false;
    }

    // store the parent path used for loading images relative to this file.

    size_t found;
    found = filename.find_last_of("/\\");
    //string parent_path = filename.substr(0,found);

    string param;
    string junk;

    int fileLength = getFileLength( file );

    m_JointInfos.clear();
    m_Bounds.clear();
    m_BaseFrames.clear();
    m_Frames.clear();
    m_AnimatedSkeleton.m_Joints.clear();
     m_Skeletons.clear();
    m_iNumFrames = 0;

    file >> param;

    while( !file.eof() )
    {
        if ( param == "MD5Version" )
        {
            file >> m_iMD5Version;
            assert( m_iMD5Version == 10 );
        }
        else if ( param == "commandline" )
        {
            file.ignore( fileLength, '\n' ); // Ignore everything else on the line
        }
        else if ( param == "numFrames" )
        {
            file >> m_iNumFrames;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "numJoints" )
        {
            file >> m_iNumJoints;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "frameRate" )
        {
            file >> m_iFramRate;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "numAnimatedComponents" )
        {
            file >> m_iNumAnimatedComponents;
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "hierarchy" )
        {
            file >> junk; // read in the '{' character
            for ( int i = 0; i < m_iNumJoints; ++i )
            {
                JointInfo joint;
                file >> joint.m_Name >> joint.m_ParentID >> joint.m_Flags >> joint.m_StartIndex;
                removeQuotes( joint.m_Name );

                m_JointInfos.push_back(joint);

                file.ignore( fileLength, '\n' );
            }
            file >> junk; // read in the '}' character
        }
        else if ( param == "bounds" )
        {
            file >> junk; // read in the '{' character
            file.ignore( fileLength, '\n' );
            for ( int i = 0; i < m_iNumFrames; ++i )
            {
                Bound bound;
                float tempX,tempY,tempZ;
                float tempX2,tempY2,tempZ2;

                file >> junk; // read in the '(' character
                file >> tempX >> tempY >> tempZ;
                file >> junk >> junk; // read in the ')' and '(' characters.
                file >> tempX2 >> tempY2 >> tempZ2;

                bound.m_Min = QVector3D(tempX,tempY,tempZ);
                bound.m_Max = QVector3D(tempX2,tempY2,tempZ2);

                m_Bounds.push_back(bound);

                file.ignore( fileLength, '\n' );
            }

            file >> junk; // read in the '}' character
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "baseframe" )
        {
            file >> junk; // read in the '{' character
            file.ignore( fileLength, '\n' );

            for ( int i = 0; i < m_iNumJoints; ++i )
            {
                float tempX,tempY,tempZ;
                float tempvecX,tempvecY,tempvecZ;
                BaseFrame baseFrame;
                file >> junk;
                file >> tempvecX >> tempvecY >> tempvecZ;
                file >> junk >> junk;
                file >> tempX >> tempY >> tempZ;
                file.ignore( fileLength, '\n' );

                baseFrame.m_Orient.setVector(tempX,tempY,tempZ);
                baseFrame.m_Pos = QVector3D(tempvecX,tempvecY,tempvecZ);

                m_BaseFrames.push_back(baseFrame);
            }
            file >> junk; // read in the '}' character
            file.ignore( fileLength, '\n' );
        }
        else if ( param == "frame" )
        {
            FrameData frame;
            file >> frame.m_iFrameID >> junk; // Read in the '{' character
            file.ignore(fileLength, '\n' );

            for ( int i = 0; i < m_iNumAnimatedComponents; ++i )
            {
                float frameData;
                file >> frameData;
                frame.m_FrameData.push_back(frameData);
            }

            m_Frames.push_back(frame);

            // Build a skeleton for this frame
            buildFrameSkeleton( m_Skeletons, m_JointInfos, m_BaseFrames, frame );

            file >> junk; // Read in the '}' character
            file.ignore(fileLength, '\n' );
        }

        file >> param;
    } // while ( !file.eof )

    // Make sure there are enough joints for the animated skeleton.
    m_AnimatedSkeleton.m_Joints.assign(m_iNumJoints, SkeletonJoint() );

    m_fFrameDuration = 1.0f / (float)m_iFramRate;
    m_fAnimDuration = ( m_fFrameDuration * (float)m_iNumFrames );
    m_fAnimTime = 0.0f;

    assert( m_JointInfos.size() == m_iNumJoints );
    assert( m_Bounds.size() == m_iNumFrames );
    assert( m_BaseFrames.size() == m_iNumJoints );
    assert( m_Frames.size() == m_iNumFrames );
    assert( m_Skeletons.size() == m_iNumFrames );

    return true;
}


void MD5Animation::removeQuotes(string& str )
{
    size_t n;
    while ( ( n = str.find('\"') ) != string::npos )
        str.erase(n,1);
}

int MD5Animation::getFileLength( std::istream& file )
{
    int pos = file.tellg();
    file.seekg(0, std::ios::end );
    int length = file.tellg();
    // Restore the position of the get pointer
    file.seekg(pos);

    return length;
}

void MD5Animation::buildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfos, const BaseFrameList& baseFrames, const FrameData& frameData )
{
    FrameSkeleton skeleton;

    for ( unsigned int i = 0; i < jointInfos.size(); ++i )
    {
        unsigned int j = 0;

        const JointInfo& jointInfo = jointInfos[i];
        // Start with the base frame position and orientation.
        SkeletonJoint animatedJoint = baseFrames[i];

        animatedJoint.m_Parent = jointInfo.m_ParentID;

        if ( jointInfo.m_Flags & 1 ) // Pos.x
        {
            animatedJoint.m_Pos.setX( frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }
        if ( jointInfo.m_Flags & 2 ) // Pos.y
        {
            animatedJoint.m_Pos.setY( frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }
        if ( jointInfo.m_Flags & 4 ) // Pos.x
        {
            animatedJoint.m_Pos.setZ( frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }
        if ( jointInfo.m_Flags & 8 ) // Orient.x
        {
            animatedJoint.m_Orient.setX( frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }
        if ( jointInfo.m_Flags & 16 ) // Orient.y
        {
            animatedJoint.m_Orient.setY(frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }
        if ( jointInfo.m_Flags & 32 ) // Orient.z
        {
            animatedJoint.m_Orient.setZ( frameData.m_FrameData[ jointInfo.m_StartIndex + j++ ]);
        }

        computeQuatW( animatedJoint.m_Orient );

        if ( animatedJoint.m_Parent >= 0 ) // Has a parent joint
        {
            SkeletonJoint& parentJoint = skeleton.m_Joints[animatedJoint.m_Parent];
            QVector3D rotPos = parentJoint.m_Orient.rotatedVector(animatedJoint.m_Pos);

            animatedJoint.m_Pos = parentJoint.m_Pos + rotPos;
            animatedJoint.m_Orient = parentJoint.m_Orient * animatedJoint.m_Orient;

            animatedJoint.m_Orient.normalize();
        }

        skeleton.m_Joints.push_back(animatedJoint);
    }

    skeletons.push_back(skeleton);
}


void MD5Animation::computeQuatW(QQuaternion &quat)
{
    float t = 1.0f - ( quat.x() * quat.x() ) - ( quat.y() * quat.y() ) - ( quat.z() * quat.z() );
    if ( t < 0.0f )
        quat.setScalar(0.0f);
    else
        quat.setScalar(-sqrtf(t));
}

void MD5Animation::update( float fDeltaTime )
{
    if ( m_iNumFrames < 1 ) return;

    m_fAnimTime += fDeltaTime;

    while ( m_fAnimTime > m_fAnimDuration ) m_fAnimTime -= m_fAnimDuration;
    while ( m_fAnimTime < 0.0f ) m_fAnimTime += m_fAnimDuration;

    // Figure out which frame we're on
    float fFramNum = m_fAnimTime * (float)m_iFramRate;
    int iFrame0 = (int)floorf( fFramNum );
    int iFrame1 = (int)ceilf( fFramNum );
    iFrame0 = iFrame0 % m_iNumFrames;
    iFrame1 = iFrame1 % m_iNumFrames;



    float fInterpolate = fmodf( m_fAnimTime, m_fFrameDuration ) / m_fFrameDuration;

    interpolateSkeletons( m_AnimatedSkeleton, m_Skeletons[iFrame0], m_Skeletons[iFrame1], fInterpolate );
    if(iFrame1<iFrame0 && !m_continuous){
        clearAnimation();
    }
}

void MD5Animation::interpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate )
{
    for ( int i = 0; i < m_iNumJoints; ++i )
    {
        SkeletonJoint& finalJoint = finalSkeleton.m_Joints[i];
        const SkeletonJoint& joint0 = skeleton0.m_Joints[i];
        const SkeletonJoint& joint1 = skeleton1.m_Joints[i];

        finalJoint.m_Parent = joint0.m_Parent;
        finalJoint.m_Pos = QQuaternion::slerp( QQuaternion(0,joint0.m_Pos), QQuaternion(0,joint1.m_Pos), fInterpolate ).vector();
        finalJoint.m_Orient = QQuaternion::slerp( joint0.m_Orient, joint1.m_Orient, fInterpolate );
    }
}

void MD5Animation::render()
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );

    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    const SkeletonJointList& joints = m_AnimatedSkeleton.m_Joints;

    // Draw the joint positions
//    glBegin( GL_POINTS );
//    {
//        for ( unsigned int i = 0; i < joints.size(); ++i )
//        {
//            glVertex3f( joints[i].m_Pos.x(),joints[i].m_Pos.y(),joints[i].m_Pos.z() );
//        }
//    }
//    glEnd();

    // Draw the bones
//    glColor3f( 0.0f, 1.0f, 0.0f );
//    glBegin( GL_LINES );
//    {
//        for ( unsigned int i = 0; i < joints.size(); ++i )
//        {
//            const SkeletonJoint& j0 = joints[i];
//            if ( j0.m_Parent != -1 )
//            {
//                const SkeletonJoint& j1 = joints[j0.m_Parent];
//                glVertex3f( j0.m_Pos.x(),j0.m_Pos.y(),j0.m_Pos.z() );
//                glVertex3f( j1.m_Pos.x(),j1.m_Pos.y(),j1.m_Pos.z() );
//            }
//        }
//    }
//    glEnd();

    glPopAttrib();
}
