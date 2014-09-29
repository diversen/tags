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

    moduleloader::includeModule('tags');
    $t = new tags();

Add a tag. 

It works with a reference and an id (unique), e.g. the 'reference'
could be 'blog' and the a blog entry id could be the 'id'

First it parses the tags, if ini setting tag_auto_add is 1 the tags will be
added to the `tags` mysql table. Then the tags will be added to the `tags_reference`
table (tags_id, reference_id, reference_name)

    $res = $t->addReference(
                $tags, 
                $reference, 
                $id
    );


Update the tags and tags_reference: 

    $res = $t->updateReference(
                $tags, 
                $reference, 
                $id
            ); 

Delete tags references:

    $res = $t->deleteReference(
                reference, 
                $id);
        }

Jquery auto complete: 

    $t = new tags();
    // include jquery script and a bit of css (which can be overridden in templates)
    $t->initJs();

Then just create an input with id `tags`
    
### As a inline module

tags module can be used in other modules with the event system. 
For adding a tag input elements to a 'create' form use: 

    // trigger form events
    event::triggerEvent(
        config::getModuleIni('content_article_events'), 
        array(
            'action' => 'form',
            'reference' => 'gallery'; // unique - e.g. module name
        )
    );

For adding a tag input (notice the parent_id wich has to be a reference to 
to e.g. the parent article) to a 'update' form use: 

    event::triggerEvent(
        config::getModuleIni('content_article_events'), 
        array(
            'action' => 'form',
            'reference' => contentArticle::$reference,
            'parent_id' => $id)
    );

Trigger an event on a insert operation. 

    $event_params = array (
        'action' => 'insert',
        'reference' => self::$reference, 
         'parent_id' => $last_insert_id);
            
    // trigger events
    event::triggerEvent(
        config::getModuleIni('content_article_events'),
        $event_params
    );

Trigger this event when delete something

    $event_params = array (
        'action' => 'delete',
        'reference' => self::$reference, 
        'parent_id' => $id);
            
    event::triggerEvent(
        config::getModuleIni('content_article_crud_events'),
        $event_params
    );

Trigger this event when updating something

    $event_params = array (
        'action' => 'update',
        'reference' => 'gallery', 
        'parent_id' => $id);
            
    event::triggerEvent(
        config::getModuleIni('content_article_crud_events'),
        $event_params
    );  

Displaying tags is done with 'view' which will echo the tags.

    $event_params = array(
        'action' => 'view',
        'reference' => 'gallery',
        'parent_id' => $val['id']);
            
    event::triggerEvent(
        config::getModuleIni('gallery_events'), 
        $event_params
    );      

Currently no method to just return the html.

