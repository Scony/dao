#include "Configuration.h"

#define CONFIGURATION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_CONFIGURATION, ConfigurationPrivate))

G_DEFINE_TYPE( Configuration, configuration, G_TYPE_OBJECT)

typedef struct _ConfigurationPrivate ConfigurationPrivate;

struct _ConfigurationPrivate
{
  int a;
};

static void configuration_finalize(GObject* configuration);

static void configuration_set_property(GObject* object, 
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *pspec);
static void configuration_get_property(GObject* object,
				       guint prop_id,
				       GValue* value,
				       GParamSpec *pspec);

static void configuration_class_init(ConfigurationClass* klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->finalize = configuration_finalize;
  gobject_class->get_property = configuration_get_property;
  gobject_class->set_property = configuration_set_property;
}

static void configuration_init(Configuration* configuration)
{
  ConfigurationPrivate* priv = CONFIGURATION_GET_PRIVATE(configuration);
}

static void configuration_finalize(GObject* object)
{
  Configuration* configuration = CONFIGURATION(object);
  ConfigurationPrivate* priv = CONFIGURATION_GET_PRIVATE(configuration);
  GObjectClass* parent_class = G_OBJECT_CLASS(configuration_parent_class);

  (*parent_class->finalize)(object);
}

Configuration* configuration_new()
{
  Configuration* configuration;

  configuration = CONFIGURATION( g_object_new( TYPE_CONFIGURATION, NULL));

  configuration->first_player = 0;
  configuration->players[0].color = 0;
  configuration->players[0].type = PLAYER_HUMAN;
  configuration->players[1].type = PLAYER_HUMAN;
  
  return configuration;
}

void configuration_read_from_gkeyfile(Configuration* configuration,
				      GKeyFile* key_file)
{
  gchar* first_player;

  first_player = g_key_file_get_string(key_file, 
				       "Game",
				       "first_move",
				       NULL);
  if(g_strcmp0(first_player, "Player2")== 0)
    {
      configuration->first_player = 1;
    }
  else
    {
      configuration->first_player = 0;
    }

}

int configuration_read_from_data(Configuration* configuration,
				  gchar* data, gsize length)
{
  GKeyFile* key_file;
  GKeyFileFlags flags;
  GError* error;

  flags = G_KEY_FILE_KEEP_COMMENTS;

  key_file = g_key_file_new();
  if (!g_key_file_load_from_data(key_file, data, length, flags, &error))
    {
      return 1;
    }
  return 0;
} 

static void configuration_get_property(GObject* object,
				       guint prop_id,
				       GValue* value,
				       GParamSpec *pspec)
{
  G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec);
}

static void configuration_set_property(GObject* object,
				       guint prop_id,
				       const GValue* value,
				       GParamSpec *pspec)
{
  G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec);
}
