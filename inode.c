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
    //bruker rekursjon
    // Sjekker om parent er noden vi leter etter, dette er base casen vår 
    if (strcmp(parent->name, name) == 0){
        return parent;
    }
    // Sjekk om noden er en mappe
    if (parent->is_directory == 0){
        return NULL;
    }
    // Sjekk alle barna rekursivt for om noen er noden vi leter etter
    for (int i = 0; i < parent->num_children; i++){
        struct inode* found_node = find_inode_by_name(parent->children[i], name);

        // Hvis noden ble funnet i et av barna, returner den
        if (found_node != NULL){
            return found_node;
        }
    }
    // Hvis ingen match ble funnet i barna, returner NULL
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

struct inode *makeInode(FILE *mft){
    struct inode *node = malloc(sizeof(struct inode));
    
    if (!node){
        printf("ikke nok minne tilgjengelig");
        return NULL;
    }
    
    fread(&node->id, sizeof(int), 1, mft);

    int name_length = 0;
    fread(&name_length, sizeof(int), 1, mft);

    node->name = malloc(name_length+1);

    if (node->name == NULL){
        printf("ikke nok minne");
        return NULL;
    }

    fread(node->name, name_length, 1, mft);
    fread(&node->is_directory, 1, 1, mft);
    
    //om mappe
    if (node->is_directory==1){
        
        fread(&node->num_children, sizeof(int), 1,mft);
        node->children = malloc(sizeof(struct inode) * node->num_children+1);

        //les child id-er 
        int *childrenIDs = (int*)malloc((node->num_children * sizeof(int)*2));

        if (childrenIDs == NULL) {
            perror("Feil ved allokering av minne for childrenIDs");
            exit(EXIT_FAILURE);
        }           
        //lagrer ider 
        for (int i = 0; i < node->num_children; i++) {
            fread(&childrenIDs[i], sizeof(int)*2, 1, mft);
        }

        //derretter leser vi alle barn inn, går ut ifra at alle er i rekkefølge etter forelder
        for (int i = 0; i<node->num_children;i++){
            
            struct inode* child = malloc(sizeof(struct inode));
            child = makeInode(mft);
            //loop igennom child id og se om node er child
            
            for (int j = 0; j < node->num_children;j++){
                if (child->id == childrenIDs[j]){
                    //printf(" child id %d ", child->id);
                    if (node->children[j] == NULL){
                        node->children[j] = child;
                       
                    }
                }
            }
        }
    }
    //om inode er fil
    else if(node->is_directory==0){
        fread(&node->filesize, sizeof(int), 1, mft);
        fread(&node->num_blocks, sizeof(int),1,mft);
        //alloker minne til blocks
        node->blocks = malloc(node->num_blocks*(sizeof(int)*2));
        for (int i = 0; i<node->num_blocks;i++){
            fread(&node->blocks[i], sizeof(int)*2, 1 , mft);
        }
    }
    return node;
}

//lager node object og lagrer i en array på posisjonen til Inode sin ID, dereretter chaines de sammen med pointere
struct inode* load_inodes( char* master_file_table )
{  
    /* to be implemented */
    FILE *mst_file;

    mst_file = fopen(master_file_table, "rb");

//dersom minne er fullt
    if (mst_file==NULL){
        printf("ikke nok minne");
        return NULL;
    }
    
    struct inode *root = makeInode(mst_file);
    fclose(mst_file);

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

