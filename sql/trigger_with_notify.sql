-- TODO find out why new.name returns null
-- listen gas_channel; <~ need to listen to this channel in the app
create or replace function on_users_update() returns trigger as
$$
begin
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' user ', new.username));
        notify gas_channel, 'user';
        return new;
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' user ', old.username));
        notify gas_channel, 'user';
        return old;
    end if;
end;
$$
    language plpgsql;

create trigger users_update
    after insert or update or delete
    on users
execute procedure on_users_update();

create or replace function on_resourcetype_update() returns trigger as
$$
begin
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resourcetype ', new.name));
        notify gas_channel, 'resourcetype';
        return new;
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resourcetype ', old.name));
        notify gas_channel, 'resourcetype';
        return old;
    end if;
end;
$$
    language plpgsql;

create trigger resourcetype_update
    after insert or update or delete
    on resourcetype
execute procedure on_resourcetype_update();

create or replace function on_resource_update() returns trigger as
$$
begin
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resource ', new.name));
        notify gas_channel, 'resource';
        return new;
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resource ', old.name));
        notify gas_channel, 'resource';
        return old;
    end if;
end;
$$
    language plpgsql;

create trigger resource_update
    after insert or update or delete
    on resource
execute procedure on_resource_update();

create or replace function on_dependency_update() returns trigger as
$$
begin
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' dependency ', new.id));
        notify gas_channel, 'dependency';
        return new;
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' dependency ', old.id));
        notify gas_channel, 'dependency';
        return old;
    end if;
end;
$$
    language plpgsql;

create trigger dependency_update
    after insert or update or delete
    on dependency
execute procedure on_dependency_update();

create or replace function on_commit_update() returns trigger as
$$
begin
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' commit ', new.id));
        notify gas_channel, 'commit';
        return new;
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' commit ', old.id));
        notify gas_channel, 'commit';
        return new;
    end if;
end;
$$
    language plpgsql;

create trigger commit_update
    after insert or update or delete
    on commits
execute procedure on_commit_update();