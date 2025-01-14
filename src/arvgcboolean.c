/* Aravis - Digital camera library
 *
 * Copyright © 2009-2012 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Emmanuel Pacaud <emmanuel@gnome.org>
 */

/**
 * SECTION: arvgcboolean
 * @short_description: Class for Boolean nodes
 */

#include <arvgcboolean.h>
#include <arvgc.h>
#include <arvmisc.h>
#include <string.h>

static GObjectClass *parent_class = NULL;

/* ArvDomNode implementation */

static const char *
arv_gc_boolean_get_node_name (ArvDomNode *node)
{
	return "Boolean";
}

static void
arv_gc_boolean_post_new_child (ArvDomNode *self, ArvDomNode *child)
{
	ArvGcBoolean *node = ARV_GC_BOOLEAN (self);

	if (ARV_IS_GC_PROPERTY_NODE (child)) {
		ArvGcPropertyNode *property_node = ARV_GC_PROPERTY_NODE (child);
		
		switch (arv_gc_property_node_get_node_type (property_node)) {
			case ARV_GC_PROPERTY_NODE_TYPE_VALUE:
			case ARV_GC_PROPERTY_NODE_TYPE_P_VALUE:
				node->value = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_ON_VALUE:
				node->on_value = property_node;
				break;
			case ARV_GC_PROPERTY_NODE_TYPE_OFF_VALUE:
				node->off_value = property_node;
				break;
			default:
				ARV_DOM_NODE_CLASS (parent_class)->post_new_child (self, child);
				break;
		}
	}
}

static void
arv_gc_boolean_pre_remove_child (ArvDomNode *self, ArvDomNode *child)
{
	g_assert_not_reached ();
}

/* ArvGcFeatureNode implementation */

static void
arv_gc_boolean_set_value_from_string (ArvGcFeatureNode *node, const char *string)
{
	arv_gc_boolean_set_value (ARV_GC_BOOLEAN (node), g_strcmp0 (string, "true") == 0);
}

static const char *
arv_gc_boolean_get_value_as_string (ArvGcFeatureNode *node)
{
	return arv_gc_boolean_get_value (ARV_GC_BOOLEAN (node)) ? "true" : "false";
}

/* ArvGcBoolean implementation */

static gint64
arv_gc_boolean_get_on_value (ArvGcBoolean *gc_boolean)
{
	if (gc_boolean->on_value != NULL)
		return arv_gc_property_node_get_int64 (gc_boolean->on_value);

	return 1;
}

static gint64
arv_gc_boolean_get_off_value (ArvGcBoolean *gc_boolean)
{
	if (gc_boolean->off_value != NULL)
		return arv_gc_property_node_get_int64 (gc_boolean->off_value);

	return 0;
}

gboolean
arv_gc_boolean_get_value (ArvGcBoolean *gc_boolean)
{
	g_return_val_if_fail (ARV_IS_GC_BOOLEAN (gc_boolean), FALSE);

	if (gc_boolean->value != NULL)
		return arv_gc_property_node_get_int64 (gc_boolean->value) == arv_gc_boolean_get_on_value (gc_boolean);

	return FALSE;
}

void
arv_gc_boolean_set_value (ArvGcBoolean *gc_boolean, gboolean v_boolean)
{
	g_return_if_fail (ARV_IS_GC_BOOLEAN (gc_boolean));

	if (gc_boolean->value != NULL)
		arv_gc_property_node_set_int64 (gc_boolean->value,
						v_boolean ?
						arv_gc_boolean_get_on_value (gc_boolean) :
						arv_gc_boolean_get_off_value (gc_boolean));
}

ArvGcNode *
arv_gc_boolean_new (void)
{
	ArvGcNode *node;

	node = g_object_new (ARV_TYPE_GC_BOOLEAN, NULL);

	return node;
}

static void
arv_gc_boolean_init (ArvGcBoolean *gc_boolean)
{
}

static void
arv_gc_boolean_finalize (GObject *object)
{
	parent_class->finalize (object);
}

static void
arv_gc_boolean_class_init (ArvGcBooleanClass *this_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (this_class);
	ArvDomNodeClass *dom_node_class = ARV_DOM_NODE_CLASS (this_class);
	ArvGcFeatureNodeClass *gc_feature_node_class = ARV_GC_FEATURE_NODE_CLASS (this_class);

	parent_class = g_type_class_peek_parent (this_class);

	object_class->finalize = arv_gc_boolean_finalize;
	dom_node_class->get_node_name = arv_gc_boolean_get_node_name;
	dom_node_class->post_new_child = arv_gc_boolean_post_new_child;
	dom_node_class->pre_remove_child = arv_gc_boolean_pre_remove_child;
	gc_feature_node_class->set_value_from_string = arv_gc_boolean_set_value_from_string;
	gc_feature_node_class->get_value_as_string = arv_gc_boolean_get_value_as_string;
}

/* ArvGcInteger interface implementation */

G_DEFINE_TYPE (ArvGcBoolean, arv_gc_boolean, ARV_TYPE_GC_FEATURE_NODE)
