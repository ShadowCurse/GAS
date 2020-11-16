-- TODO find out why new.name returns null
-- listen gas_channel; <~ need to listen to this channel in the app
create or replace function on_developer_update() returns trigger as
$$
begin
    -- log
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' developer ', new.name));
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' developer ', old.name));
    end if;
    -- notify
    notify gas_channel, 'developer';
    return new;
end;
$$
    language plpgsql;

create trigger developer_update
    after insert or update or delete
    on developer
execute procedure on_developer_update();

create or replace function on_resourcetype_update() returns trigger as
$$
begin
    -- log
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resourcetype ', new.name));
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resourcetype ', old.name));
    end if;
    -- notify
    notify gas_channel, 'resourcetype';
    return new;
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
    -- log
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resource ', new.name));
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' resource ', old.name));
    end if;
    -- notify
    notify gas_channel, 'resource';
    return new;
end;
$$
    language plpgsql;

create trigger developer_update
    after insert or update or delete
    on resource
execute procedure on_resource_update();

create or replace function on_dependency_update() returns trigger as
$$
begin
    -- log
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' dependency ', new.id));
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' dependency ', old.id));
    end if;
    -- notify
    notify gas_channel, 'dependency';
    return new;
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
    -- log
    if lower(tg_op) = 'insert' or lower(tg_op) = 'update'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' commit ', new.id));
    elseif lower(tg_op) = 'delete'
    then
        insert into logs (date, description)
        values ((select current_date), concat(lower(tg_op), ' commit ', old.id));
    end if;
    -- notify
    notify gas_channel, 'commit';
    return new;
end;
$$
    language plpgsql;

create trigger commit_update
    after insert or update or delete
    on commits
execute procedure on_commit_update();