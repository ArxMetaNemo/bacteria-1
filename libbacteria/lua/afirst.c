#include"libcrypto.h"
 //
 //    /* forward declaration for the iterator function */
 //
 //static int dir_iter (lua_State *L);
 //    
 //static int l_dir (lua_State *L) {
 //      const char *path = luaL_checkstring(L, 1);
 //    
 //      /* create a userdatum to store a DIR address */
 //      DIR **d = (DIR **)lua_newuserdata(L, sizeof(DIR *));
 //    
 //      /* set its metatable */
 //      luaL_getmetatable(L, "LuaBook.dir");
 //      lua_setmetatable(L, -2);
 //    
 //      /* try to open the given directory */
 //      *d = opendir(path);
 //      if (*d == NULL)  /* error opening the directory? */
 //        luaL_error(L, "cannot open %s: %s", path,
 //                                            strerror(errno));
 //    
 //      /* creates and returns the iterator function
 //         (its sole upvalue, the directory userdatum,
 //         is already on the stack top */
 //      lua_pushcclosure(L, dir_iter, 1);
 //      return 1;
 //}
 //static int dir_iter (lua_State *L) {
 //      DIR *d = *(DIR **)lua_touserdata(L, lua_upvalueindex(1));
 //      struct dirent *entry;
 //      if ((entry = readdir(d)) != NULL) {
 //        lua_pushstring(L, entry->d_name);
 //        return 1;
 //      }
 //      else return 0;  /* no more values to return */
 //}
 //
 //static int dir_gc (lua_State *L) {
 //      DIR *d = *(DIR **)lua_touserdata(L, 1);
 //      if (d) closedir(d);
 //      return 0;
 //}
 //int luaopen_dir (lua_State *L) {
 //      luaL_newmetatable(L, "LuaBook.dir");
 //
 //      /* set its __gc field */
 //      lua_pushstring(L, "__gc");
 //      lua_pushcfunction(L, dir_gc);
 //      lua_settable(L, -3);
 //
 //      /* register the `dir' function */
 //      lua_pushcfunction(L, l_dir);
 //      lua_setglobal(L, "dir");
 //
 //      return 0;
 //}
 //
 //




int start_lua (void) {
      char buff[256];
      int error;
      lua_State *L = lua_open();   /* opens Lua */
//      luaopen_base(L);             /* opens the basic library */
//      luaopen_table(L);            /* opens the table library */
//      luaopen_io(L);               /* opens the I/O library */
//      luaopen_string(L);           /* opens the string lib. */
//      luaopen_math(L);             /* opens the math lib. */
//luaopen_dir(L);
luaopen_cryptocoins(L);
luaL_openlibs(L);
lua_loadscript(L,"check.lua");
      while (fgets(buff, sizeof(buff), stdin) != NULL) {
        error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
                lua_pcall(L, 0, 0, 0);
        if (error) {
          fprintf(stderr, "%s", lua_tostring(L, -1));
          lua_pop(L, 1);  /* pop error message from the stack */
        }
      }

      lua_close(L);
      return 0;
}
