#include <h3/h3api.h>
#include <erl_nif.h>

static ERL_NIF_TERM make_list_from_array(ErlNifEnv *env, CellBoundary *gp) {
    ERL_NIF_TERM list = enif_make_list(env, 0);

    for(int i = gp->numVerts - 1; i >= 0; i--) {
        ERL_NIF_TERM lat = enif_make_double(env, radsToDegs(gp->verts[i].lat));
        ERL_NIF_TERM lng = enif_make_double(env, radsToDegs(gp->verts[i].lng));

        ERL_NIF_TERM point = enif_make_tuple2(env, lat, lng);

        list = enif_make_list_cell(env, point, list);
    }

    
    return list;
};

// check what is contained under erlnifenv
static ERL_NIF_TERM make_h3_error(ErlNifEnv* env, H3Error err)
{
    return enif_make_tuple2(env, enif_make_atom(env, "error"), err);
}

// used to return tuple with {:ok, {data}}
static ERL_NIF_TERM make_h3_success_return(ErlNifEnv* env, ERL_NIF_TERM term){
    return enif_make_tuple2(env, enif_make_atom(env, "ok"), term);
}

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
    latLng.lat = degsToRads(latLng.lat);
    latLng.lng = degsToRads(latLng.lng);
    H3Error err = latLngToCell(&latLng, res, &out);
	H3Error er = h3ToString(out, buf, sizeof(buf));


    /* return result */
    // return enif_make_uint64(env, out);
    return make_h3_success_return(env, enif_make_string(env, buf, ERL_NIF_UTF8));
}

static ERL_NIF_TERM cell_to_lat_lng(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	H3Index h3;
    H3Error err;
	LatLng g;
	char buf[17];

    if(!enif_get_string(env, argv[0], buf, sizeof(buf), ERL_NIF_UTF8))
        return enif_make_badarg(env);

    err = stringToH3(buf, &h3);
    if(err != E_SUCCESS)
        return make_h3_error(env, err);

    err = cellToLatLng(h3, &g);
    if(err != E_SUCCESS)
        return make_h3_error(env, err);
    g.lat = radsToDegs(g.lat);
    g.lng = radsToDegs(g.lng);

    return make_h3_success_return(env,  enif_make_tuple2(env, enif_make_double(env, g.lat),  enif_make_double(env, g.lng)));
}

static ERL_NIF_TERM cell_to_boundary(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
	H3Index h3;
    H3Error err;
	CellBoundary gp;
    char buf[17];

    if(!enif_get_string(env, argv[0], buf, sizeof(buf), ERL_NIF_UTF8))
        return enif_make_badarg(env);
    
    err = stringToH3(buf, &h3);
    if(err != E_SUCCESS)
        return make_h3_error(env, err);

    err = cellToBoundary(h3, &gp);
    if(err != E_SUCCESS)
        return make_h3_error(env, err);

	return enif_make_tuple2(env, enif_make_int(env, gp.numVerts), make_list_from_array(env, &gp));
}


/** @brief maximum number of hexagons in k-ring */
static ERL_NIF_TERM max_grid_disk_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    int k;
    int64_t out;

    if (!enif_get_int(env, argv[0], &k))
        return enif_make_badarg(env);
    H3Error err = maxGridDiskSize(k, &out);
    if (err != E_SUCCESS)
        return make_h3_error(env, err);
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
