/* 
* Raytracing Engine
* Copyright (C) 2011-2012 Cyrille Favreau <cyrille_favreau@hotmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

/*
* Author: Cyrille Favreau <cyrille_favreau@hotmail.com>
*
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Consts.h"
#include "Logging.h"
#include "FileMarshaller.h"

const std::string SCENEINFO = "SCENEINFO";
const std::string PRIMITIVE = "PRIMITIVE";
const std::string MATERIAL  = "MATERIAL";
const std::string TEXTURE   = "TEXTURE";

FileMarshaller::FileMarshaller()
{
}


FileMarshaller::~FileMarshaller(void)
{
}

void FileMarshaller::readTexture( GPUKernel& kernel, const std::string& line )
{
   std::string value;
   size_t i( strlen(TEXTURE.c_str())+1 );
   int textureId(MATERIAL_NONE);
   std::string textureFilename;
   size_t c(0);
   while( i<line.length() )
   {
      if( line[i] == ';' ) 
      {
         switch(c)
         {
            case  0: textureId       = static_cast<int>(atoi( value.c_str() )); break;
            case  1: textureFilename = value; break;
         }
         value = "";
         ++i;
         c++;
      }
      value += line[i];
      ++i;
   }

   if( value != "" )
   {
      textureFilename = value;
   }

   if( textureId!=MATERIAL_NONE )
   {
      LOG_INFO(3, "Loading texture " << textureFilename << " into slot " << textureId );
      if( !kernel.loadTextureFromFile(textureId, textureFilename) )
      {
         LOG_ERROR("Failed to load texture " << textureFilename << " into slot " << textureId );
      }
   }
}

void FileMarshaller::readSceneInfo( GPUKernel& kernel, const std::string& line )
{
   SceneInfo sceneInfo;
   std::string value;
   size_t i( strlen(SCENEINFO.c_str())+1 );
   size_t c(0);
   while( i<line.length() )
   {
      if( line[i] == ';' ) 
      {
         switch(c)
         {
            case  0: sceneInfo.width.x  = static_cast<int>(atoi( value.c_str() )); break;
            case  1: sceneInfo.height.x = static_cast<int>(atoi( value.c_str() )); break;
            case  2: sceneInfo.graphicsLevel.x = static_cast<int>(atoi( value.c_str() )); break;
            case  3: sceneInfo.nbRayIterations.x = static_cast<int>(atoi( value.c_str() )); break;
            case  4: sceneInfo.transparentColor.x = static_cast<float>(atof( value.c_str() )); break;
            case  5: sceneInfo.viewDistance.x = static_cast<float>(atof( value.c_str() )); break;
            case  6: sceneInfo.shadowIntensity.x = static_cast<float>(atof( value.c_str() )); break;
            case  7: sceneInfo.width3DVision.x = static_cast<float>(atof( value.c_str() )); break;
            case  8: sceneInfo.backgroundColor.x = static_cast<float>(atof( value.c_str() )); break;
            case  9: sceneInfo.backgroundColor.y = static_cast<float>(atof( value.c_str() )); break;
            case 10: sceneInfo.backgroundColor.z = static_cast<float>(atof( value.c_str() )); break;
            case 11: sceneInfo.backgroundColor.w = static_cast<float>(atof( value.c_str() )); break;
            case 12: sceneInfo.renderingType.x = static_cast<int>(atoi( value.c_str() )); break;
            case 13: sceneInfo.renderBoxes.x = static_cast<int>(atoi( value.c_str() )); break;
            case 14: sceneInfo.pathTracingIteration.x = static_cast<int>(atoi( value.c_str() )); break;
            case 15: sceneInfo.maxPathTracingIterations.x = static_cast<int>(atoi( value.c_str() )); break;
            case 16: sceneInfo.misc.x = static_cast<int>(atoi( value.c_str() )); break;
            case 17: sceneInfo.misc.y = static_cast<int>(atoi( value.c_str() )); break;
         }
         value = "";
         ++i;
         c++;
      }
      value += line[i];
      ++i;
   }

   sceneInfo.width.x = kernel.getSceneInfo().width.x;
   sceneInfo.height.x = kernel.getSceneInfo().height.x;
   kernel.setSceneInfo( sceneInfo );
}

void FileMarshaller::readPrimitive( GPUKernel& kernel, const std::string& line, Vertex& min, Vertex& max, const Vertex& center )
{
   Primitive primitive = {0.f};
   std::string value;
   size_t i( strlen(PRIMITIVE.c_str())+1 );
   int c(0);
   int boxId(0);

   while( i<line.length() )
   {
      if( line[i] == ';' ) 
      {
         switch(c)
         {
            case  0: primitive.type.x   = static_cast<int>(atoi( value.c_str() )); break;

            case  1: primitive.p0.x = static_cast<float>(atof( value.c_str() )); break;
            case  2: primitive.p0.y = static_cast<float>(atof( value.c_str() )); break;
            case  3: primitive.p0.z = static_cast<float>(atof( value.c_str() )); break;

            case  4: primitive.p1.x = static_cast<float>(atof( value.c_str() )); break;
            case  5: primitive.p1.y = static_cast<float>(atof( value.c_str() )); break;
            case  6: primitive.p1.z = static_cast<float>(atof( value.c_str() )); break;

            case  7: primitive.p2.x = static_cast<float>(atof( value.c_str() )); break;
            case  8: primitive.p2.y = static_cast<float>(atof( value.c_str() )); break;
            case  9: primitive.p2.z = static_cast<float>(atof( value.c_str() )); break;

            case 10: primitive.n0.x = static_cast<float>(atof( value.c_str() )); break;
            case 11: primitive.n0.y = static_cast<float>(atof( value.c_str() )); break;
            case 12: primitive.n0.z = static_cast<float>(atof( value.c_str() )); break;

            case 13: primitive.n1.x = static_cast<float>(atof( value.c_str() )); break;
            case 14: primitive.n1.y = static_cast<float>(atof( value.c_str() )); break;
            case 15: primitive.n1.z = static_cast<float>(atof( value.c_str() )); break;

            case 16: primitive.n2.x = static_cast<float>(atof( value.c_str() )); break;
            case 17: primitive.n2.y = static_cast<float>(atof( value.c_str() )); break;
            case 18: primitive.n2.z = static_cast<float>(atof( value.c_str() )); break;

            case 19: primitive.size.x = static_cast<float>(atof( value.c_str() )); break;
            case 20: primitive.size.y = static_cast<float>(atof( value.c_str() )); break;
            case 21: primitive.size.z = static_cast<float>(atof( value.c_str() )); break;

            case 22: primitive.materialId.x   = static_cast<int>(atoi( value.c_str() )); break;

            // Texture coordinates
            case 23: primitive.vt0.x = static_cast<float>(atof( value.c_str() )); break;
            case 24: primitive.vt0.y = static_cast<float>(atof( value.c_str() )); break;
            case 25: primitive.vt0.z = static_cast<float>(atof( value.c_str() )); break;

            case 26: primitive.vt1.x = static_cast<float>(atof( value.c_str() )); break;
            case 27: primitive.vt1.y = static_cast<float>(atof( value.c_str() )); break;
            case 28: primitive.vt1.z = static_cast<float>(atof( value.c_str() )); break;

            case 29: primitive.vt2.x = static_cast<float>(atof( value.c_str() )); break;
            case 30: primitive.vt2.y = static_cast<float>(atof( value.c_str() )); break;
            case 31: primitive.vt2.z = static_cast<float>(atof( value.c_str() )); break;
         }
         value = "";
         ++i;
         c++;
      }
      value += line[i];
      ++i;
   }

   if( value.length() != 0 )
   {
      primitive.vt2.z = static_cast<float>(atof( value.c_str() ));
   }

   int n = kernel.addPrimitive( static_cast<PrimitiveType>(primitive.type.x) );
   kernel.setPrimitive(
      n, 
      center.x+primitive.p0.x, center.y+primitive.p0.y, center.z+primitive.p0.z,
      center.x+primitive.p1.x, center.y+primitive.p1.y, center.z+primitive.p1.z,
      center.x+primitive.p2.x, center.y+primitive.p2.y, center.z+primitive.p2.z,
      primitive.size.x, primitive.size.y, primitive.size.z,
      primitive.materialId.x );

   kernel.setPrimitiveNormals( n, primitive.n0, primitive.n1, primitive.n2 );
   kernel.setPrimitiveTextureCoordinates( n, primitive.vt0, primitive.vt1, primitive.vt2 );

   Vertex pmin, pmax;
   pmin.x = std::min(std::min( primitive.p0.x, primitive.p1.x ), primitive.p2.x );
   pmin.y = std::min(std::min( primitive.p0.y, primitive.p1.y ), primitive.p2.y );
   pmin.z = std::min(std::min( primitive.p0.z, primitive.p1.z ), primitive.p2.z );
   pmax.x = std::max(std::max( primitive.p0.x, primitive.p1.x ), primitive.p2.x );
   pmax.y = std::max(std::max( primitive.p0.y, primitive.p1.y ), primitive.p2.y );
   pmax.z = std::max(std::max( primitive.p0.z, primitive.p1.z ), primitive.p2.z );

   min.x = ( pmin.x < min.x ) ? pmin.x : min.x;
   min.y = ( pmin.y < min.y ) ? pmin.y : min.y;
   min.z = ( pmin.z < min.z ) ? pmin.z : min.z;

   max.x = ( pmax.x > max.x ) ? pmax.x : max.x;
   max.y = ( pmax.y > max.y ) ? pmax.y : max.y;
   max.z = ( pmax.z > max.z ) ? pmax.z : max.z;
}

void FileMarshaller::readMaterial( GPUKernel& kernel, const std::string& line, const int materialId )
{
   Material material;
   memset(&material,0,sizeof(Material));
   std::string value;
   size_t i( strlen(MATERIAL.c_str())+1 );
   size_t c(0);
   size_t boxId(0);

   while( i<line.length() )
   {
      if( line[i] == ';' ) 
      {
         switch(c)
         {
            case  0: material.color.x   = static_cast<float>(atof( value.c_str() )); break;
            case  1: material.color.y   = static_cast<float>(atof( value.c_str() )); break;
            case  2: material.color.z   = static_cast<float>(atof( value.c_str() )); break;
            case  3: material.color.w   = static_cast<float>(atof( value.c_str() )); break;
            case  4: material.innerIllumination.x = static_cast<float>(atof( value.c_str() )); break;
            case  5: material.innerIllumination.y = 40000.f; //static_cast<float>(atof( value.c_str() )); break;
            case  6: material.innerIllumination.z = 10.f*kernel.getSceneInfo().viewDistance.x; //static_cast<float>(atof( value.c_str() )); break;
            case  7: material.innerIllumination.w = static_cast<float>(atof( value.c_str() )); break;
            case  8: material.reflection.x        = static_cast<float>(atof( value.c_str() )); break;
            case  9: material.refraction.x        = static_cast<float>(atof( value.c_str() )); break;
            case 10: material.specular.x          = static_cast<float>(atof( value.c_str() )); break;
            case 11: material.specular.y          = static_cast<float>(atof( value.c_str() )); break;
            case 12: material.specular.z          = static_cast<float>(atof( value.c_str() )); break;
            case 13: material.specular.w          = static_cast<float>(atof( value.c_str() )); break;
            case 14: material.attributes.x       = static_cast<int>(atoi( value.c_str() )); break;
            case 15: material.attributes.y       = static_cast<int>(atoi( value.c_str() )); break;
            case 16: material.attributes.z       = static_cast<int>(atoi( value.c_str() )); break;
            case 17: material.attributes.w       = static_cast<int>(atoi( value.c_str() )); break;
            case 18: material.transparency.x      = static_cast<float>(atof( value.c_str() )); break;
            case 19: material.textureMapping.x  = static_cast<int>(atoi( value.c_str() )); break;
            case 20: material.textureMapping.y  = static_cast<int>(atoi( value.c_str() )); break;
            case 21: 
               material.textureMapping.z  = static_cast<int>(atoi( value.c_str() )); 
               if( kernel.getTextureInformation(material.textureMapping.z).buffer==nullptr )
               {
                  LOG_ERROR("Texture " << material.textureMapping.z << " could not be loaded. Default color will be used for material " << materialId );
                  material.textureMapping.z = MATERIAL_NONE;
               }
               else
               {
                  if( material.textureMapping.z!=MATERIAL_NONE)
                  {
                     LOG_INFO(1, "Texture " << material.textureMapping.z << " assigned to material " << materialId );
                  }
               }
               break; 
            case 22: material.textureMapping.w  = static_cast<int>(atoi( value.c_str() )); break;
         }
         value = "";
         ++i;
         c++;
      }
      value += line[i];
      ++i;
   }

   if( value.length() != 0 )
   {
      material.textureMapping.w = static_cast<int>(atoi( value.c_str() ));
   }

   // Force sky and ground textures
   kernel.setMaterial(
      materialId,
      material.color.x, material.color.y, material.color.z, material.color.w,
      material.reflection.x, material.refraction.x,
      (material.attributes.y==1), 
      (material.attributes.z==1), 
      material.attributes.w,
      material.transparency.x,
      material.textureIds.x, material.textureIds.y, material.textureIds.z,
      material.specular.x, material.specular.y, material.specular.z,
      material.innerIllumination.x,material.innerIllumination.y,material.innerIllumination.z,
      (material.attributes.x==1));
   
   LOG_INFO(3, "Added material [" << materialId << "] " << 
      "( " << material.color.x << ", " << material.color.y << ", " << material.color.z << ") " <<
      "( " << material.specular.x << ", " << material.specular.y << ", " << material.specular.z << ") " <<
      ", Texture ID= " << material.textureMapping.z );
}

Vertex FileMarshaller::loadFromFile( GPUKernel& kernel, const std::string& filename, const Vertex& center, const float scale )
{
   LOG_INFO(1, "Loading 3D scene from " << filename );

   Vertex returnValue;
   Vertex min = {  kernel.getSceneInfo().viewDistance.x,  kernel.getSceneInfo().viewDistance.x,  kernel.getSceneInfo().viewDistance.x };
   Vertex max = { -kernel.getSceneInfo().viewDistance.x, -kernel.getSceneInfo().viewDistance.x, -kernel.getSceneInfo().viewDistance.x };

   std::ifstream myfile;
   myfile.open(filename.c_str());
   if( myfile.is_open() ) 
   {
      int materialId(0);
      //kernel.cleanup();

      while( !myfile.eof() ) 
      {
         std::string line;
         std::getline(myfile, line);
         if( line.find(SCENEINFO) == 0 )
         {
            //readSceneInfo( kernel, line );
         }
         else if( line.find(MATERIAL) == 0 )
         {
            readMaterial( kernel, line, materialId );
            materialId++;
         }
         else if( line.find(PRIMITIVE) == 0 )
         {
            readPrimitive( kernel, line, min, max, center );
         }
         else if( line.find(TEXTURE) == 0 )
         {
            readTexture( kernel, line );
         }
      }
   }
   myfile.close();
   returnValue.x = fabs( max.x - min.x );
   returnValue.y = fabs( max.y - min.y );
   returnValue.z = fabs( max.z - min.z );

   float ratio = scale / returnValue.y;
   kernel.scalePrimitives( ratio, 0, NB_MAX_BOXES );

   LOG_INFO(1, "Object size: " << returnValue.x << ", " << returnValue.y << ", " << returnValue.z );

   //kernel.setOptimalNbOfBoxes( 5*static_cast<int>(sqrt(static_cast<float>(kernel.getNbActivePrimitives()))));

   return returnValue;
}

void FileMarshaller::saveToFile( GPUKernel& kernel, const std::string& filename)
{
   int frame(0);
   LOG_INFO(1, "Saving 3D scene to " << filename );
   std::ofstream myfile;
   myfile.open(filename.c_str());
   if( myfile.is_open() ) 
   {

      std::map<int,Material*> materials;
      std::map<int,std::string> textures;

      LOG_INFO(1, "Scene information " );

      SceneInfo sceneInfo = kernel.getSceneInfo();
      // Scene
      myfile << SCENEINFO << ";" <<
         sceneInfo.width.x << ";" <<
         sceneInfo.height.x << ";" <<
         sceneInfo.graphicsLevel.x << ";" <<
         sceneInfo.nbRayIterations.x << ";" <<
         sceneInfo.transparentColor.x << ";" <<
         sceneInfo.viewDistance.x << ";" <<
         sceneInfo.shadowIntensity.x << ";" <<
         sceneInfo.width3DVision.x << ";" <<
         sceneInfo.backgroundColor.x << ";" <<
         sceneInfo.backgroundColor.y << ";" <<
         sceneInfo.backgroundColor.z << ";" <<
         sceneInfo.backgroundColor.w << ";" <<
         sceneInfo.renderingType.x << ";" <<
         sceneInfo.renderBoxes.x << ";" <<
         sceneInfo.pathTracingIteration.x << ";" <<
         sceneInfo.maxPathTracingIterations.x << ";" <<
         sceneInfo.misc.x << ";" <<
         sceneInfo.misc.y << std::endl;

      // Primitives
      int nbPrimitives = kernel.getNbActivePrimitives();
      LOG_INFO(1,nbPrimitives << " primitives");

      // Identify used materials
      std::map<int,int> materialIndexMapping;
      for( int i(0); i<nbPrimitives; ++i )
      {
         CPUPrimitive* primitive = kernel.getPrimitive(i);
         materialIndexMapping[primitive->materialId] = 0;
      }

      int index(0);
      std::map<int,int>::iterator itmim=materialIndexMapping.begin();
      while(itmim!=materialIndexMapping.end())
      {
         (*itmim).second = index;
         ++itmim;
         ++index;
      }


      for( int i(0); i<nbPrimitives; ++i )
      {
         CPUPrimitive* primitive = kernel.getPrimitive(i);
         Material* m = kernel.getMaterial( primitive->materialId );
         bool isLight = (m != NULL && m->innerIllumination.x!=0.f );
         if( !isLight )
         {
            myfile << PRIMITIVE << ";" <<  
               primitive->type << ";" <<
               primitive->p0.x << ";" <<
               primitive->p0.y << ";" <<
               primitive->p0.z << ";" <<
               primitive->p1.x << ";" <<
               primitive->p1.y << ";" <<
               primitive->p1.z << ";" <<
               primitive->p2.x << ";" <<
               primitive->p2.y << ";" <<
               primitive->p2.z << ";" <<
               primitive->n0.x << ";" <<
               primitive->n0.y << ";" <<
               primitive->n0.z << ";" <<
               primitive->n1.x << ";" <<
               primitive->n1.y << ";" <<
               primitive->n1.z << ";" <<
               primitive->n2.x << ";" <<
               primitive->n2.y << ";" <<
               primitive->n2.z << ";" <<
               primitive->size.x << ";" <<
               primitive->size.y << ";" <<
               primitive->size.z << ";" <<
               materialIndexMapping[primitive->materialId] << ";" <<
               primitive->vt0.x << ";" <<
               primitive->vt0.y << ";" <<
               primitive->vt0.z << ";" <<
               primitive->vt1.x << ";" <<
               primitive->vt1.y << ";" <<
               primitive->vt1.z << ";" <<
               primitive->vt2.x << ";" <<
               primitive->vt2.y << ";" <<
               primitive->vt2.z << std::endl;

            materials[primitive->materialId] = kernel.getMaterial(primitive->materialId);
         }
      }

      // Determine textures in use
      std::map<int,Material*>::const_iterator ittiu(materials.begin());
      index=0;
      while( ittiu!=materials.end() )
      {
         Material* material = (*ittiu).second;
         if( material->textureMapping.z!=MATERIAL_NONE )
         {
            textures[material->textureMapping.z] = kernel.getTextureFilename(material->textureMapping.z);
         }
         ++ittiu;
      }

      // Write Textures
      std::map<int,std::string>::const_iterator itt=textures.begin();
      while( itt!=textures.end() )
      {
         if( filename.length() != 0 )
         {
            myfile << TEXTURE << ";" << (*itt).first << ";" << (*itt).second << std::endl;
         }
         ++itt;
      }

      // Write Materials
      LOG_INFO(1, materials.size() << " materials");
      std::map<int,Material*>::const_iterator itm(materials.begin());
      index=0;
      while( itm!=materials.end() )
      {
         Material* material = (*itm).second;
         myfile << MATERIAL << ";" <<
            material->color.x << ";" <<
            material->color.y << ";" <<
            material->color.z << ";" <<
            material->color.w << ";" <<
            material->innerIllumination.x << ";" <<
            material->innerIllumination.y << ";" <<
            material->innerIllumination.z << ";" <<
            material->innerIllumination.w << ";" <<
            material->reflection.x << ";" <<
            material->refraction.x << ";" <<
            material->specular.x << ";" <<
            material->specular.y << ";" <<
            material->specular.z << ";" <<
            material->specular.w << ";" <<
            material->attributes.x << ";" <<
            material->attributes.y << ";" <<
            material->attributes.z << ";" <<
            material->attributes.w << ";" <<
            material->transparency.x << ";" <<
            material->textureMapping.x << ";" <<
            material->textureMapping.y << ";" <<
            material->textureMapping.z << ";" <<
            material->textureMapping.w << ";" << 
            std::endl;
         ++itm;
      }

      myfile.close();
   }
}
