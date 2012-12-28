#include "Configuration.h"

#define BUFFER_SIZE 64

#define CONFIGURATION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_CONFIGURATION, ConfigurationPrivate))

G_DEFINE_TYPE( Configuration, configuration, G_TYPE_OBJECT)

typedef struct _ConfigurationPrivate ConfigurationPrivate;

struct _ConfigurationPrivate
{
  int a;
};

enum
  {
    SIGNAL_CHANGED,
    N_SIGNALS
  };

guint dao_configuration_signals[N_SIGNALS];


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

  dao_configuration_signals[SIGNAL_CHANGED] = 
    g_signal_newv("changed",
		    G_TYPE_FROM_CLASS(gobject_class),
		    G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
		    G_SIGNAL_NO_HOOKS,
		    NULL,
		    NULL,
		    NULL,
		    g_cclosure_marshal_VOID__VOID,
		    G_TYPE_NONE,
		    0,
		    NULL);
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

int configuration_read_from_gkeyfile(Configuration* configuration,
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

  //Players
  for(int i = 0; i < 2; i++)
    {
      gchar player_name_buf[BUFFER_SIZE];
      PlayerConfiguration* player;
      gchar* player_type;

      g_snprintf(player_name_buf, BUFFER_SIZE, "Player%d", i+1);
      g_print("%s\n", player_name_buf);

      player = configuration->players + i;
      player->color = g_key_file_get_integer(key_file, 
					     player_name_buf,
					     "color",
					     NULL);
      player_type = g_key_file_get_string(key_file,
					  player_name_buf,
					  "type",
					  NULL);
      if(g_strcmp0(player_type, "human") == 0)
	{
	  player->type = PLAYER_HUMAN;
	}
      else if (g_strcmp0(player_type, "computer") == 0)
	{
	  player->type = PLAYER_COMPUTER;
	}
      else
	{
	  g_print("Unknown player type");
	  return 1;
	}
      
      if (player->type == PLAYER_COMPUTER)
	{
	  gchar* player_algorithm;
	  player_algorithm = g_key_file_get_string(key_file,
						   player_name_buf,
						   "algorithm",
						   NULL);

	  if (g_strcmp0(player_algorithm, "random") == 0)
	    {
	      player->algorithm = ALGORITHM_RANDOM;
	    }
	  else if(g_strcmp0(player_algorithm, "hill climber") == 0)
	    {
	      player->algorithm = ALGORITHM_HILL_CLIMBER;
	    }
	  else
	    {
	      g_print("Unknown algorithm\n");
	      return 1;
	    }
	}
    }

  g_print("%d", configuration->first_player);

g_signal_emit(configuration, dao_configuration_signals[SIGNAL_CHANGED], 0);
  return 0;
}

int configuration_read_from_data(Configuration* configuration,
				  gchar* data, gsize length)
{
  GKeyFileFlags flags;
  GError* error;

  flags = G_KEY_FILE_KEEP_COMMENTS;

  configuration->key_file = g_key_file_new();
  if (!g_key_file_load_from_data(configuration->key_file,
				   data, length, flags, &error))
    {
      return 1;
    }

  return configuration_read_from_gkeyfile(configuration, 
				     configuration->key_file);
}

int configuration_read_from_file(Configuration* configuration,
				   char* filename)
{
  GKeyFileFlags flags;
  GError* error;

  flags = G_KEY_FILE_KEEP_COMMENTS;

  configuration->key_file = g_key_file_new();
g_print("Reading from file");
  if (!g_key_file_load_from_file(configuration->key_file,
				   filename, flags, &error))
    {
      return 1;
    }
g_print("Read from file");

  configuration_read_from_gkeyfile(configuration, 
				     configuration->key_file);
  return 0;
}


gchar* configuration_get_data(Configuration* configuration,
				gsize* length, GError **error)
{
return g_key_file_to_data(configuration->key_file,
			    length, error);
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
