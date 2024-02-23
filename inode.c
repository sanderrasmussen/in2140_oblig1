    #include "allocation.h"
#include "inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* The number of bytes in a block.
 * Do not change.
 */
#define BLOCKSIZE 4096

/* The lowest unused node ID.
 * Do not change.
 */
static int num_inode_ids = 0;

/* This helper function computes the number of blocks that you must allocate
 * on the simulated disk for a give file system in bytes. You don't have to use
 * it.
 * Do not change.
 */
static int blocks_needed( int bytes )
{
    int blocks = bytes / BLOCKSIZE;
    if( bytes % BLOCKSIZE != 0 )
        blocks += 1;
    return blocks;
}

/* This helper function returns a new integer value when you create a new inode.
 * This helps you do avoid inode reuse before 2^32 inodes have been created. It
 * keeps the lowest unused inode ID in the global variable num_inode_ids.
 * Make sure to update num_inode_ids when you have loaded a simulated disk.
 * Do not change.
 */
static int next_inode_id( )
{
    int retval = num_inode_ids;
    num_inode_ids += 1;
    return retval;
}

struct inode* create_file( struct inode* parent, char* name, int size_in_bytes )
{




    /*alocere og lage array som den fyller */
    /* to be implemented */
    return NULL;
}

struct inode* create_dir( struct inode* parent, char* name )
{
    /* to be implemented */
    return NULL;
}

struct inode* find_inode_by_name( struct inode* parent, char* name )
{
    /* to be implemented */
    return NULL;
}

static int verified_delete_in_parent( struct inode* parent, struct inode* node )
{
    /* to be implemented */
    return 0;
}

int is_node_in_parent( struct inode* parent, struct inode* node )
{
    /* to be implemented */


    return 0;
}

int delete_file( struct inode* parent, struct inode* node )
{
    /* to be implemented */
    return 0;
}

int delete_dir( struct inode* parent, struct inode* node )
{
    /* to be implemented */
    return 0;
}

struct inode* load_inodes( char* master_file_table )
{  
    /* to be implemented */
    FILE *mst_file;
    mst_file = fopen(master_file_table, "r");

    if (!mst_file){
        perror("error opening file");
        return NULL;
    };
    
    //create root node
    struct inode *root = (struct inode*)malloc(sizeof(struct inode));

    if (!root){//not enough space
        perror("not enough space for root node ");
        fclose(mst_file);
        return NULL;
    };

    //first read the entire file and store all in an array
    fseek(mst_file, 0, SEEK_END); // Sett filposisjon til slutten av filen
    long file_size = ftell(mst_file); // Finn filstørrelsen
    fseek(mst_file, 0, SEEK_SET); // Sett filposisjon tilbake til starten

    char *data = malloc(file_size + 4); // Alloker minne for filinnhold
    fread(data, 2,file_size, mst_file);

    data[file_size] = '\0'; // Nullterminer data-arrayen
    //fill the rote node with the first data
    //printf("%02X", data[8]); //dette printer en hex 0x00
    //ser ut som at data[] chars automatisk blir convertert til ascii eller int når jeg setter de til det
    //printf("%s", &data[8]); 

    int current = 0 ;//to keep track of where in the file we shal read

    //id er første 4 bites
    //while (current<file_size){

        //lager rotnode 
        int bitesToRead = 4;
        int id =0;
        for (current; current<bitesToRead;current++){
            root->id += data[current];
        }

        //lengde av navn er 4 bites
        bitesToRead+=4;
        int nameLength =0;
        for (current; current<bitesToRead;current++){
            nameLength+=data[current];
        }
        root->name = malloc(nameLength);

        //navn er 2 byte
        bitesToRead+= nameLength;
        printf("%d", nameLength);
        char name[nameLength];
      
        for (current; current<bitesToRead;current++){
            if (name==NULL){
                strcpy(name,&data[current]); //dersom ingen navn
            }
            else{
                strcat(name, &data[current]); //concatiner med navn
            }
        }
        root->name = name;
         
        //dir er altid en byte
        root->is_directory = data[current];
        current+=1;

        //num children er alltid 4 byte
        bitesToRead+=4;
        for (current; current<bitesToRead;current++){
            root->num_children+=data[current];
        }

        //alloker minne til children pointers
        root->children = malloc(root->num_children * sizeof(struct inode));
        
   // }
   //leser resten av inodene
        struct inode *parent = root;

    while (current<file_size){


        //lager nyInode 
        struct inode *node = (struct inode*)malloc(sizeof(struct inode));

        int bitesToRead = 4;
        int id =0;
        for (current; current<bitesToRead;current++){
            node->id += data[current];
        }

        //lengde av navn er 4 bites
        bitesToRead+=4;
        int nameLength =0;
        for (current; current<bitesToRead;current++){
            nameLength+=data[current];
        }
        node->name = malloc(nameLength);

        //navnlengde er 2 byte
        bitesToRead+= nameLength;
        char name[nameLength];
      
        for (current; current<bitesToRead;current++){
            if (name==NULL){
                strcpy(name,&data[current]); //dersom ingen navn
            }
            else{
                strcat(name, &data[current]); //concatiner med navn
            }
        }
        node->name = name;
         
        //dir er altid en byte
        node->is_directory = data[current];
        current+=1;

        //en if block for fil og en for mappe

        //LAGER MAPPE
        if(node->is_directory==1){
             //num children er alltid 4 byte
            bitesToRead+=4;
            for (current; current<bitesToRead;current++){
                node->num_children+=data[current]; //beregner antall children
            }

            //alloker minne til children pointers
            node->children = malloc(root->num_children * sizeof(struct inode)); //alokerer children array
        }

        //LAGER FIL
        if(node->is_directory==0){
            //data[blocknummer]
            //CURRENT+=1;
            
            //leser filesize
            int bytesize = 4;
            for (current; current<bytesize;current++){
                node->filesize+=data[current]; 
            }
            






            //node->blocks = malloc(node->num_blocks * sizeof(filesize));

        }


    }

    //printf("%c", data[8]);
    
    //printf("%d",root->id );
    fclose(mst_file);
    
    free(data);

    return root;
}

/* The function save_inode is a recursive functions that is
 * called by save_inodes to store a single inode on disk,
 * and call itself recursively for every child if the node
 * itself is a directory.
 */
static void save_inode( FILE* file, struct inode* node )
{
    if( !node ) return;

    int len = strlen( node->name ) + 1;

    fwrite( &node->id, 1, sizeof(int), file );
    fwrite( &len, 1, sizeof(int), file );
    fwrite( node->name, 1, len, file );
    fwrite( &node->is_directory, 1, sizeof(char), file );
    if( node->is_directory )
    {
        fwrite( &node->num_children, 1, sizeof(int), file );
        for( int i=0; i<node->num_children; i++ )
        {
            struct inode* child = node->children[i];
            size_t id = child->id;
            fwrite( &id, 1, sizeof(size_t), file );
        }

        for( int i=0; i<node->num_children; i++ )
        {
            struct inode* child = node->children[i];
            save_inode( file, child );
        }
    }
    else
    {
        fwrite( &node->filesize, 1, sizeof(int), file );
        fwrite( &node->num_blocks, 1, sizeof(int), file );
        for( int i=0; i<node->num_blocks; i++ )
        {
            fwrite( &node->blocks[i], 1, sizeof(size_t), file );
        }
    }
}

void save_inodes( char* master_file_table, struct inode* root )
{
    if( root == NULL )
    {
        fprintf( stderr, "root inode is NULL\n" );
        return;
    }

    FILE* file = fopen( master_file_table, "w" );
    if( !file )
    {
        fprintf( stderr, "Failed to open file %s\n", master_file_table );
        return;
    }

    save_inode( file, root );

    fclose( file );
}

/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

/* Do not change.
 */
void debug_fs( struct inode* node )
{
    if( node == NULL ) return;
    for( int i=0; i<indent; i++ )
        printf("  ");

    if( node->is_directory )
    {
        printf("%s (id %d)\n", node->name, node->id );
        indent++;
        for( int i=0; i<node->num_children; i++ )
        {
            struct inode* child = (struct inode*)node->children[i];
            debug_fs( child );
        }
        indent--;
    }
    else
    {
        printf("%s (id %d size %db blocks ", node->name, node->id, node->filesize );
        for( int i=0; i<node->num_blocks; i++ )
        {
            printf("%d ", (int)node->blocks[i]);
        }
        printf(")\n");
    }
}

/* Do not change.
 */
void fs_shutdown( struct inode* inode )
{
    if( !inode ) return;

    if( inode->is_directory )
    {
        for( int i=0; i<inode->num_children; i++ )
        {
            fs_shutdown( inode->children[i] );
        }
    }

    if( inode->name )     free( inode->name );
    if( inode->children ) free( inode->children );
    if( inode->blocks )   free( inode->blocks );
    free( inode );
}

