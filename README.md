### Tags

Tag module to tag any other entry like blog, and article, or question. 

configuration: 

    ; admin setting. How many tags to display on a page
    tags_per_page = 10
    ; allow anybody to add new tags
    tags_auto_add = 1
    ; who can add, edit, delete tags
    tags_allow_edit = "admin"
    ; min char for a tag 
    tags_min_chars = 0

### Manual usage:

include the module: 

    use modules\tags\module as tags

    // update tags tag connect to a reference and a parent_id

    $params = array (
        'action' => 'update',
        'reference' => 'blog', 
        'parent_id' => $id,
        'published' => 1);

    $t = new tags();
    $t->events($params);

    // delete a tag connected to a reference and a parent_id

        $params = array(
            'action' => 'delete',
            'reference' => 'blog',
            'parent_id' => $id);
    
        $t = new tags();
        $t->events($params);



    // add a form field for adding tags (autocomplete)
        $params = array(
            'reference' => 'id',
            'parent_id' => $id,
            'action' => 'form'
        );        

        $t = new tags();
        $t->events($params);