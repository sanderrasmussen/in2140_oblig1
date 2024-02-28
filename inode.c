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

// ---YUAN SIN KODE START-----  
// hjelpemetode
// hjelpemetode til create_file og create_dir
void add_child_to_dir(struct inode *parent, struct inode *child)
{
    struct inode **new_children = (struct inode **)realloc(parent->children, sizeof(struct inode *) * (parent->num_children + 1));
    if (new_children == NULL)
    {

        printf("add to parent realloc feil!");
        return;
    }

    new_children[parent->num_children] = child;
    parent->children = new_children;
    parent->num_children++;
}

struct inode *create_file(struct inode *parent, char *name, int size_in_bytes)
{
    /* to be implemented */

    // If there is a file or directory with the same name there already,return NULL
    if (find_inode_by_name(parent, name) != NULL)
    {
        return NULL;
    }

    // allocate memory space to file_inode
    struct inode *file_inode = (struct inode *)malloc(sizeof(struct inode));
    if (file_inode == NULL)
    {
        printf("file_inode malloc feil!");
        return NULL;
    }
    // init Directory
    file_inode->id = next_inode_id(); // get new inode ID
    file_inode->name = strdup(name);
    if (file_inode->name == NULL)
    {
        printf("name i file_inode malloc feil!");
        free(file_inode);
        return NULL;
    }

    file_inode->is_directory = 0; // is file type
    file_inode->num_children = 0;
    file_inode->children = NULL;
    file_inode->filesize = size_in_bytes;
    file_inode->num_blocks = blocks_needed(size_in_bytes);
    //  Allocate blocks on simulated disk
    file_inode->blocks = (size_t *)malloc(sizeof(size_t) * file_inode->num_blocks);
    // cheak: if memory allocation failed
    if (file_inode->blocks == NULL)
    {
        printf("blocks i file_inode malloc feil!");
        free(file_inode->name);
        free(file_inode);
        return NULL;
    }

    for (int i = 0; i < file_inode->num_blocks; i++)
    {
        int block = allocate_block();
        if (block == -1)
        {
            // Allocation failed, release allocated blocks and return NULL
            for (int j = 0; j < i; j++)
            {
                printf("blocks: $d i file_inode malloc feil!", i);
                free_block(file_inode->blocks[j]); // array's value is Int (block num)
            }
            free(file_inode->name);
            free(file_inode->blocks);
            free(file_inode);
            return NULL;
        }
        file_inode->blocks[i] = block;
    }

    if (parent != NULL)
    {
        add_child_to_dir(parent, file_inode);
    }
    return file_inode;
}

struct inode *create_dir(struct inode *parent, char *name)
{
    /* to be implemented */

    // If there is a file or directory with the same name there already, return NULL
    if (find_inode_by_name(parent, name) != NULL)
    {
        printf("found samme navn i en mappe");
        return NULL;
    }
    // creat a diractory, allocate memory space to dir_inode
    struct inode *dir_inode = (struct inode *)malloc(sizeof(struct inode));
    if (dir_inode == NULL)
    {
        printf("dir_inode malloc feil!");
        return NULL;
    }

    dir_inode->id = next_inode_id();
    dir_inode->name = strdup(name);
    if (dir_inode->name == NULL)
    {
        printf("name i dir_inode malloc feil!");
        free(dir_inode);
        return NULL;
    }

    dir_inode->is_directory = 1;
    dir_inode->num_children = 0;
    dir_inode->children = NULL;
    dir_inode->filesize = 0;
    dir_inode->num_blocks = 0;
    dir_inode->blocks = malloc(0); // her kan vi bruke "NULL"

    if (parent != NULL)
    {
        add_child_to_dir(parent, dir_inode);
    }
    return dir_inode;
}

struct inode *find_inode_by_name(struct inode *parent, char *name)
{
    /* to be implemented */
    // checks all inodes that are referenced directly from the parent inode
    if (parent == NULL)
    {
        return NULL;
    }
    if (parent->is_directory == 0){
        return NULL;
    }
    for (int i = 0; i < parent->num_children; i++)
    {
        struct inode *child = parent->children[i];
        if (strcmp(child->name, name) == 0) // true
        {
            if (parent->children[i]!=NULL){
                return parent->children[i];
            }
            
        }
    }
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