#include <h3/h3api.h>
#include <erl_nif.h>


static ERL_NIF_TERM
make_h3idx(ErlNifEnv * env, H3Index index)
{
    return enif_make_uint64(env, index);
}

static ERL_NIF_TERM 
enif_make_lat_lng(ErlNifEnv *env, LatLng *g)
{
    g->lat = radsToDegs(g->lat);
    g->lng = radsToDegs(g->lng);
	return enif_make_tuple2(env, enif_make_double(env, g->lat),  enif_make_double(env, g->lng));
}

static ERL_NIF_TERM lat_lng_to_cell(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    LatLng latLng;
    int arity;
    const ERL_NIF_TERM *elems;

    int res;
    H3Index out;
	char buf[17];
	

    /* argv[0] = {lat, lng} */
    if (!enif_get_tuple(env, argv[0], &arity, &elems) || arity != 2)
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[0], &latLng.lat))
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[1], &latLng.lng))
        return enif_make_badarg(env);

    /* argv[1] = resolution */
    if (!enif_get_int(env, argv[1], &res))
        return enif_make_badarg(env);

    /* call H3 */
    H3Error err = latLngToCell(&latLng, res, &out);
	H3Error er = h3ToString(out, buf, sizeof(buf));


    /* return result */
    // return enif_make_uint64(env, out);
    return enif_make_string(env, buf, ERL_NIF_UTF8);
}

static ERL_NIF_TERM cell_to_lat_lng(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	H3Index h3;
	LatLng g;
	char buf[17];

    if(!enif_get_string(env, argv[0], buf, sizeof(buf), ERL_NIF_UTF8))
        return enif_make_badarg(env);

	
    	H3Error err = cellToLatLng(h3, &g);
	
	return enif_make_tuple2(env, enif_make_double(env, g.lat),  enif_make_double(env, g.lng));

}

static ERL_NIF_TERM cell_to_boundary(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	H3Index h3;
	CellBoundary gp;
    char buf[17];

    if(!enif_get_string(env, argv[0], buf, sizeof(buf), ERL_NIF_UTF8))
        return enif_make_badarg(env);
	
	H3Error err = cellToBoundary(h3, &gp);
	return enif_make_tuple2(env, enif_make_int(env, gp.numVerts), enif_make_lat_lng(env, gp.verts));
}


/** @brief maximum number of hexagons in k-ring */
static ERL_NIF_TERM max_grid_disk_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    int k;
    int64_t out;

    if (!enif_get_int(env, argv[0], &k))
        return enif_make_badarg(env);
    H3Error err = maxGridDiskSize(k, &out);
    return enif_make_int64(env, out);
}

/** @brief hexagons neighbors in all directions, assuming no pentagons */
static ERL_NIF_TERM grid_disk_unsafe(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   H3Index origin, out;
   int k;
   if(!enif_get_uint64(env, argv[0], &origin))
        return enif_make_badarg(env);
    if(!enif_get_int(env, argv[1], &k))
        return enif_make_badarg(env);
    H3Error err = gridDiskUnsafe(origin, k, &out);
    return enif_make_uint64(env, out);
}

/** @brief hexagons neighbors in all directions */
static ERL_NIF_TERM grid_disk(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   H3Index origin, out;
   int k;
   if(!enif_get_uint64(env, argv[0], &origin))
        return enif_make_badarg(env);
    
    H3Error err = gridDisk(origin, k, &out);
    return enif_make_uint64(env, out);
}

/** @brief hexagon neighbors in all directions, reporting distance from origin
 */
static ERL_NIF_TERM grid_disk_distances(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   H3Index origin, out;
   int k, distances;
   if(!enif_get_uint64(env, argv[0], &origin))
        return enif_make_badarg(env);
    if(!enif_get_int(env, argv[1], &k))
        return enif_make_badarg(env);
    
    H3Error err = gridDiskDistances(origin, k, &out, &distances);
    return enif_make_tuple2(env, make_h3idx(env, out), enif_make_int(env, distances));
}
/** @brief maximum number of hexagons in k-ring */
static ERL_NIF_TERM max_grid_ring_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    int k;
    int64_t out;

    if (!enif_get_int(env, argv[0], &k))
        return enif_make_badarg(env);
    H3Error err = maxGridRingSize(k, &out);
    return enif_make_int64(env, out);
}

/** @brief hollow hexagon ring k distance from origin */
static ERL_NIF_TERM grid_ring_unsafe(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   H3Index origin, out;
   int k;
   if(!enif_get_uint64(env, argv[0], &origin))
        return enif_make_badarg(env);

  if(!enif_get_int(env, argv[1], &k))
    return enif_make_badarg(env);
    
    H3Error err = gridRingUnsafe(origin, k, &out);
    return make_h3idx(env, out);
}

/** @brief hollow hexagon ring k distance from origin */
static ERL_NIF_TERM grid_ring(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   H3Index origin, out;
   int k;
   if(!enif_get_uint64(env, argv[0], &origin))
        return enif_make_badarg(env);
    if(!enif_get_int(env, argv[1], &k))
        return enif_make_badarg(env);
    
    H3Error err = gridRing(origin, k, &out);
    return enif_make_uint64(env, out);
}

static ErlNifFunc nif_funcs[] = {
    {"lat_lng_to_cell", 2, lat_lng_to_cell},
    {"cell_to_lat_lng", 1, cell_to_lat_lng},
    {"cell_to_boundary", 1, cell_to_boundary},
    {"max_grid_disk_size", 1, max_grid_disk_size},
    {"grid_disk_unsafe", 2, grid_disk_unsafe},
    {"grid_disk", 2, grid_disk},
    {"grid_disk_distances", 2, grid_disk_distances},
    {"max_grid_ring_size", 1, max_grid_ring_size},
    {"grid_ring_unsafe", 2, grid_ring_unsafe},
    {"grid_ring", 2, grid_ring}
};

ERL_NIF_INIT(Elixir.ExH3, nif_funcs, NULL, NULL, NULL, NULL);
