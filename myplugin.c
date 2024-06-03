#include <git2.h>
#include <stdint.h>
#include <inttypes.h>
#include <thunarx/thunarx.h>
#include <gtk/gtk.h>
#include <pango/pango.h>
#include <harfbuzz/hb.h>
#include <gmodule.h> 

typedef struct _MyPlugin {
    GObject parent_instance;
} MyPlugin;

typedef struct _MyPluginClass {
    GObjectClass parent_class;
} MyPluginClass;

G_DEFINE_TYPE(MyPlugin, my_plugin, G_TYPE_OBJECT)

static void my_plugin_update_status(ThunarxRenamerProvider *provider, ThunarxFileInfo *file_info, GtkWidget *statusbar) {
    const gchar *path = thunarx_file_info_get_uri(file_info);

    // Check if the path is a git repository
    git_libgit2_init();
    git_repository *repo = NULL;
    int error = git_repository_open_ext(&repo, path, GIT_REPOSITORY_OPEN_NO_SEARCH, NULL);
    if (error != 0) {
        // Not a git repository
        git_libgit2_shutdown();
        return;
    }

    // Get the current branch
    git_reference *head = NULL;
    git_repository_head(&head, repo);
    const char *branch = git_reference_shorthand(head);

    // Get status
    git_status_list *status_list = NULL;
    git_status_options status_opts = GIT_STATUS_OPTIONS_INIT;
    status_opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    git_status_list_new(&status_list, repo, &status_opts);
    size_t status_count = git_status_list_entrycount(status_list);

    // Build status message
    gchar *status_message = g_strdup_printf("Branch: %s, Changes: %zu", branch, status_count);

    // Update Thunar status bar
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "git"), status_message);

    // Clean up
    g_free(status_message);
    git_status_list_free(status_list);
    git_reference_free(head);
    git_repository_free(repo);
    git_libgit2_shutdown();
}

static void my_plugin_class_init(MyPluginClass *klass) {
    // Initialize the plugin class
}

static void my_plugin_init(MyPlugin *self) {
    // Initialize the plugin instance
}

struct _ThunarxProviderFactory *

G_MODULE_EXPORT thunar_extension_initialize(ThunarxProviderFactory *provider) {
    thunarx_provider_factory_get_type(provider, my_plugin_get_type());
    thunarx_provider_factory_list_providers(provider, thunarx_renamer_provider_get_type(), my_plugin_update_status);
}

G_MODULE_EXPORT void thunar_extension_shutdown(void) {
    // Code to release resources, if necessary
}

G_MODULE_EXPORT ThunarxProviderFactory* thunar_extension_invoke_factory(void) {
    return THUNARX_PROVIDER_FACTORY(thunarx_provider_factory_get_type());
}
